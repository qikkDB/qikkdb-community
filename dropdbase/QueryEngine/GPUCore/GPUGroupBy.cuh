#pragma once

#include <cstdint>
#include <limits>
#include <memory>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../Context.h"
#include "GPUMemory.cuh"
#include "GPUBinary.cuh"
#include "GPUReconstruct.cuh"

#include "ErrorFlagSwapper.h"
#include "cuda_ptr.h"
#include "IGroupBy.h"
#include "AggregationFunctions.cuh"
#include "GPUTypes.h"


/// Universal null key calculator
template <typename K>
__device__ __host__ constexpr K getEmptyValue()
{
    static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
                  "Unsupported data type of key (in function getEmptyValue)");
    return std::numeric_limits<K>::lowest();
}

/// Generic atomic CAS (compare and set) for any 4 and 8 bytes long data type.
template <typename T>
__device__ T genericAtomicCAS(T* address, T compare, T val)
{
    static_assert(sizeof(T) == 8 || sizeof(T) == 4,
                  "genericAtomicCAS is working only for 4 Bytes and 8 Bytes long data types");
    if (sizeof(T) == 8)
    {
        uint64_t old =
            atomicCAS(reinterpret_cast<cuUInt64*>(address), *(reinterpret_cast<cuUInt64*>(&compare)),
                      *(reinterpret_cast<cuUInt64*>(&val)));
        return *(reinterpret_cast<T*>(&old));
    }
    else if (sizeof(T) == 4)
    {
        int32_t old = atomicCAS(reinterpret_cast<int32_t*>(address), *(reinterpret_cast<int32_t*>(&compare)),
                                *(reinterpret_cast<int32_t*>(&val)));
        return *(reinterpret_cast<T*>(&old));
    }
    else
    {
        return T{0};
    }
}


/// GROUP BY Kernel processes input (inKeys and inValues). New keys from inKeys are added
/// to the hash table and values from inValues are aggregated.
/// <param name="keys">key buffer of the hash table</param>
/// <param name="values">value buffer of the hash table</param>
/// <param name="keyOccurrenceCount">key occurrences in the hash table</param>
/// <param name="maxHashCount">size of the hash table (max. number of keys)</param>
/// <param name="inKeys">input buffer with keys</param>
/// <param name="inValues">input buffer with values</param>
/// <param name="dataElementCount">count of rows in input</param>
/// <param name="errorFlag">GPU pointer to error flag</param>
template <typename AGG, typename K, typename V>
__global__ void group_by_kernel(K* keys,
                                V* values,
                                int64_t* valuesNullMask,
                                int64_t* keyOccurrenceCount,
                                const int32_t loweredMaxHashCount,
                                K* inKeys,
                                V* inValues,
                                const int32_t dataElementCount,
                                const int32_t arrayMultiplier,
                                int32_t* errorFlag,
                                int64_t* inKeysNullMask,
                                int64_t* inValuesNullMask)
{
    const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    for (int32_t i = idx; i < dataElementCount &&
                          *errorFlag != static_cast<int32_t>(QueryEngineErrorType::GPU_HASH_TABLE_FULL);
         i += stride)
    {
        const int32_t bitMaskIdx = (i / (sizeof(int8_t) * 8));
        const int32_t shiftIdx = (i % (sizeof(int8_t) * 8));
        const bool nullKey = (inKeysNullMask != nullptr) && ((inKeysNullMask[bitMaskIdx] >> shiftIdx) & 1);
        const bool nullValue =
            (inValuesNullMask != nullptr) && ((inValuesNullMask[bitMaskIdx] >> shiftIdx) & 1);
        int32_t foundIndex = -1;

        if (!nullKey)
        {
            const int32_t hash = abs(static_cast<int32_t>(inKeys[i])) %
                                 loweredMaxHashCount; // TODO maybe improve hashing for float

            for (int32_t j = 0; j < loweredMaxHashCount; j++)
            {
                // Calculate hash - use type conversion because of float
                const int32_t index = ((hash + j) % loweredMaxHashCount) + 1;

                // Check if key is not empty and key is not equal to the currently inserted key
                if (keys[index] != getEmptyValue<K>() && keys[index] != inKeys[i])
                {
                    continue;
                }

                // If key is empty
                if (keys[index] == getEmptyValue<K>())
                {
                    // Compare key at index with Empty and if equals, store there inKey
                    K old = genericAtomicCAS<K>(&keys[index], getEmptyValue<K>(), inKeys[i]);

                    // Check if some other thread stored different key to this index
                    if (old != getEmptyValue<K>() && old != inKeys[i])
                    {
                        continue; // Try to find another index
                    }

                    /* // Explanation - all conditions explicitly defined
                    if (old != getEmptyValue<K>())
                    {
                        if (old == inKeys[i])
                        {
                            foundIndex = index;
                            printf("Existing key: %d\n", inKeys[i]);
                            break;
                        }
                        else // old != inKeys[i]
                        {
                            printf("Lost race: %d\n", inKeys[i]);
                            continue; // try to find another index
                        }
                    }
                    else // old == getEmptyValue<K>()
                    {
                        if (keys[index] == inKeys[i])
                        {
                            foundIndex = index;
                            printf("Added key: %d\n", inKeys[i]);
                            break;
                        }
                        else // keys[index] != inKeys[i]
                        {
                            printf("This will never happen: %d\n", inKeys[i]);
                            continue; // try to find another index
                        }
                    }*/
                }
                else if (keys[index] != inKeys[i])
                {
                    continue; // try to find another index
                }

                // The key was added or found as already existing
                foundIndex = index;
                break;
            }
        }
        else
        {
            foundIndex = 0;
            keys[foundIndex] = K{0};
        }

        // If no index was found - the hash table is full
        // else if we found a valid index
        if (foundIndex == -1)
        {
            atomicExch(errorFlag, static_cast<int32_t>(QueryEngineErrorType::GPU_HASH_TABLE_FULL));
        }
        else
        {
            // Use aggregation of values on the bucket and the corresponding counter
            if (!nullValue)
            {
                if (values)
                {
                    AGG{}(values + foundIndex * arrayMultiplier + threadIdx.x % arrayMultiplier, inValues[i]);
                    if (valuesNullMask[foundIndex])
                    {
                        valuesNullMask[foundIndex] = 0;
                    }
                }
                if (keyOccurrenceCount)
                {
                    atomicAdd(reinterpret_cast<cuUInt64*>(keyOccurrenceCount + foundIndex * arrayMultiplier +
                                                          threadIdx.x % arrayMultiplier),
                              1);
                }
            }
        }
    }
}

// TODO remake to filter colConst
/// Helper kernel for mask creation from key buffer of the hash table.
template <typename K>
__global__ void is_bucket_occupied_kernel(int64_t* occupancyMask, K* keys, int32_t maxHashCount)
{
    const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    for (int32_t i = idx; i < maxHashCount; i += stride)
    {
        occupancyMask[i] = (keys[i] != getEmptyValue<K>());
    }
}

/// Kernel for merging multiplied arrays (values with AGG nonatomic functions and occurrences with +=)
template <typename AGG, typename V, bool USE_VALUES, bool USE_OCCURRENCES>
__global__ void kernel_merge_multiplied_arrays(V* mergedValues,
                                               int64_t* mergedOccurrences,
                                               V* inValues,
                                               int64_t* inOccurrences,
                                               int64_t* occupancyMask,
                                               const int32_t maxHashCount,
                                               const int32_t arrayMultiplier)
{
    const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    for (int32_t i = idx; i < maxHashCount; i += stride)
    {
        if (occupancyMask[i])
        {
            if (USE_VALUES)
            {
                mergedValues[i] = AGG::template getInitValue<V>();
            }
            if (USE_OCCURRENCES)
            {
                mergedOccurrences[i] = 0;
            }
            for (int32_t j = 0; j < arrayMultiplier; j++)
            {
                if (USE_VALUES)
                {
                    AGG{}.template nonatomic<V>(mergedValues + i, inValues[i * arrayMultiplier + j]);
                }
                if (USE_OCCURRENCES)
                {
                    mergedOccurrences[i] += inOccurrences[i * arrayMultiplier + j];
                }
            }
        }
    }
}


/// Merge multiplied arrays and return as tuple of two cuda_ptr-s
/// <param name="inValues">input n-plicated array of values</param>
/// <param name="inOccurrences">input n-plicated array of occurrences</param>
/// <param name="maxHashCount">size of one hash table</param>
template <typename AGG, typename V, bool USE_VALUES, bool USE_OCCURRENCES>
std::tuple<cuda_ptr<V>, cuda_ptr<int64_t>> MergeMultipliedArrays(V* inValues,
                                                                 int64_t* inOccurrences,
                                                                 int64_t* occupancyMask,
                                                                 const int32_t maxHashCount,
                                                                 const int32_t arrayMultiplier)
{
    // Merge multipied arrays (values and occurrences)
    cuda_ptr<V> mergedValues(maxHashCount);
    cuda_ptr<int64_t> mergedOccurrences(maxHashCount);
    kernel_merge_multiplied_arrays<AGG, V, USE_VALUES, USE_OCCURRENCES>
        <<<Context::getInstance().calcGridDim(maxHashCount), Context::getInstance().getBlockDim()>>>(
            mergedValues.get(), mergedOccurrences.get(), inValues, inOccurrences, occupancyMask,
            maxHashCount, arrayMultiplier);
    CheckCudaError(cudaGetLastError());
    return std::make_tuple<cuda_ptr<V>, cuda_ptr<int64_t>>(std::move(mergedValues), std::move(mergedOccurrences));
}


/// GROUP BY generic class for single numeric key.
template <typename AGG, typename O, typename K, typename V>
class GPUGroupBy : public IGroupBy
{
private:
    static constexpr bool USE_VALUES = std::is_same<AGG, AggregationFunctions::min>::value ||
                                       std::is_same<AGG, AggregationFunctions::max>::value ||
                                       std::is_same<AGG, AggregationFunctions::sum>::value ||
                                       std::is_same<AGG, AggregationFunctions::avg>::value;

    static constexpr bool USE_KEY_OCCURRENCES = std::is_same<AGG, AggregationFunctions::avg>::value ||
                                                std::is_same<AGG, AggregationFunctions::count>::value;

    static constexpr bool DIRECT_VALUES = std::is_same<AGG, AggregationFunctions::min>::value ||
                                          std::is_same<AGG, AggregationFunctions::max>::value ||
                                          std::is_same<AGG, AggregationFunctions::sum>::value;

    /// Key buffer of the hash table
    K* keys_ = nullptr;

    /// Value buffer of the hash table
    V* values_ = nullptr;
    int64_t* valuesNullMask_ = nullptr;

    /// Count of values aggregated per key (helper buffer of the hash table)
    int64_t* keyOccurrenceCount_ = nullptr;

    /// Size of the keys buffer of hash table (max. count of unique keys)
    const size_t keyBufferSize_;
    /// Multiplier for value buffer size
    const int32_t valueBufferMultiplier_;
    /// Error flag swapper for error checking after kernel runs
    ErrorFlagSwapper errorFlagSwapper_;

public:
    /// Create GPUGroupBy object and allocate a hash table (buffers for key, values and key occurrence counts)
    /// <param name="maxHashCount">size of the hash table (max. count of unique keys)</param>
    GPUGroupBy(const int32_t maxHashCount, const int32_t keySizeMultiplier)
    : keyBufferSize_(std::min(static_cast<size_t>(maxHashCount) * keySizeMultiplier, GB_BUFFER_SIZE_MAX) + 1), // + 1 for NULL key
      valueBufferMultiplier_((keySizeMultiplier > GB_VALUE_BUFFER_DEFAULT_MULTIPLIER) ?
                                 1 :
                                 GB_VALUE_BUFFER_DEFAULT_MULTIPLIER / keySizeMultiplier)
    {
        // Compute size for value and occurrences buffer
        const size_t valueBufferSize = keyBufferSize_ * valueBufferMultiplier_;
        try
        {
            CudaLogBoost::getInstance(CudaLogBoost::debug) << "Alloc " << keyBufferSize_ << " for keys\n";
            GPUMemory::alloc(&keys_, keyBufferSize_);
            if (USE_VALUES)
            {
                CudaLogBoost::getInstance(CudaLogBoost::debug) << "Alloc " << valueBufferSize << " for values\n";
                GPUMemory::alloc(&values_, valueBufferSize);
                GPUMemory::allocAndSet(&valuesNullMask_, 1, keyBufferSize_);
            }
            if (USE_KEY_OCCURRENCES)
            {
                CudaLogBoost::getInstance(CudaLogBoost::debug) << "Alloc " << valueBufferSize << " for occurrences\n";
                GPUMemory::allocAndSet(&keyOccurrenceCount_, 0, valueBufferSize);
            }
        }
        catch (...)
        {
            if (keys_)
            {
                GPUMemory::free(keys_);
            }
            if (values_)
            {
                GPUMemory::free(values_);
            }
            if (valuesNullMask_)
            {
                GPUMemory::free(valuesNullMask_);
            }
            if (keyOccurrenceCount_)
            {
                GPUMemory::free(keyOccurrenceCount_);
            }
            throw;
        }
        GPUMemory::fillArray(keys_, getEmptyValue<K>(), keyBufferSize_);
        if (USE_VALUES)
        {
            GPUMemory::fillArray(values_, AGG::template getInitValue<V>(), valueBufferSize);
        }
    }

    /// Create GPUGroupBy object with existing keys (allocate whole new hash table)
    /// <param name="maxHashCount">size of the hash table (max. count of unique keys)</param>
    /// <param name="keys">GPU buffer with existing keys (will be copied to a new buffer)</param>
    GPUGroupBy(const int32_t maxHashCount, K* keys, bool firstKeyIsNull)
    : GPUGroupBy(maxHashCount - (firstKeyIsNull ? 1 : 0), 1)
    {
        // keys_ has value at index 0 reserved for NULL key
        GPUMemory::copyDeviceToDevice(keys_ + (firstKeyIsNull ? 0 : 1), keys, maxHashCount);
    }

    ~GPUGroupBy()
    {
        GPUMemory::free(keys_);
        if (USE_VALUES)
        {
            GPUMemory::free(values_);
            GPUMemory::free(valuesNullMask_);
        }
        if (USE_KEY_OCCURRENCES)
        {
            GPUMemory::free(keyOccurrenceCount_);
        }
    }

    GPUGroupBy(const GPUGroupBy&) = delete;
    GPUGroupBy& operator=(const GPUGroupBy&) = delete;

    /// Run GROUP BY on one input buffer - callable repeatedly on the blocks of the input columns
    /// <param name="inKeys">input buffer with keys</param>
    /// <param name="inValues">input buffer with values</param>
    /// <param name="dataElementCount">row count to process</param>
    void ProcessBlock(K* inKeys, V* inValues, int32_t dataElementCount, int64_t* inKeysNullMask = nullptr, int64_t* inValuesNullMask = nullptr)
    {
        if (dataElementCount > 0)
        {
            group_by_kernel<AGG>
                <<<Context::getInstance().calcGridDim(dataElementCount), Context::getInstance().getBlockDim()>>>(
                    keys_, values_, valuesNullMask_, keyOccurrenceCount_, keyBufferSize_ - 1,
                    inKeys, inValues, dataElementCount, valueBufferMultiplier_,
                    errorFlagSwapper_.GetFlagPointer(), inKeysNullMask, inValuesNullMask);
            errorFlagSwapper_.Swap();
        }
    }

    /// Get the size of the hash table (max. count of unique keys)
    /// <returns>size of the hash table</returns>
    int32_t GetMaxHashCount()
    {
        return keyBufferSize_;
    }

    /// Create memory-wasting null mask for keys - one 1 at [0], other zeros
    cuda_ptr<int64_t> CreateKeyNullMask()
    {
        cuda_ptr<int64_t> keyNullMask(keyBufferSize_, 0);
        GPUMemory::memset(keyNullMask.get(), 1, 1);
        return keyNullMask;
    }

    /// Reconstruct needed raw fields (do not calculate final results yet)
    /// Reconstruct keys, values and key occurrence counts separately
    /// <param name="keys">output buffer to fill with reconstructed keys</param>
    /// <param name="values">output buffer to fill with reconstructed values</param>
    /// <param name="occurrences">not used buffer if using operations MIN, MAX or SUM - nullptr can be used</param>
    /// <param name="elementCount">ouptut buffer to fill with element count (one int32_t number)</param>
    void ReconstructRawNumbers(K* keys, int64_t* keysNullMask, V* values, int64_t* valuesNullMask, int64_t* occurrences, int32_t* elementCount)
    {
        cuda_ptr<int64_t> occupancyMask(keyBufferSize_, 0);
        is_bucket_occupied_kernel<<<Context::getInstance().calcGridDim(keyBufferSize_),
                                    Context::getInstance().getBlockDim()>>>(occupancyMask.get(),
                                                                            keys_, keyBufferSize_);

        cuda_ptr<int64_t> keysNullMaskInput = CreateKeyNullMask();

        GPUReconstruct::reconstructCol(keys, elementCount, keys_, occupancyMask.get(), keyBufferSize_);
        GPUReconstruct::reconstructCol(keysNullMask, elementCount, keysNullMaskInput.get(),
                                       occupancyMask.get(), keyBufferSize_);

        // Merge multipied arrays (values and occurrences)
        std::tuple<cuda_ptr<V>, cuda_ptr<int64_t>> mergedArrays =
            MergeMultipliedArrays<AGG, V, USE_VALUES, USE_KEY_OCCURRENCES>(values_, keyOccurrenceCount_,
                                                                           occupancyMask.get(), keyBufferSize_,
                                                                           valueBufferMultiplier_);
        cuda_ptr<V> mergedValues = std::move(std::get<0>(mergedArrays));
        cuda_ptr<int64_t> mergedOccurrences = std::move(std::get<1>(mergedArrays));

        if (USE_VALUES)
        {
            GPUReconstruct::reconstructCol(values, elementCount, mergedValues.get(),
                                           occupancyMask.get(), keyBufferSize_);
            GPUReconstruct::reconstructCol(valuesNullMask, elementCount, valuesNullMask_,
                                           occupancyMask.get(), keyBufferSize_);
        }
        if (USE_KEY_OCCURRENCES)
        {
            GPUReconstruct::reconstructCol(occurrences, elementCount, mergedOccurrences.get(),
                                           occupancyMask.get(), keyBufferSize_);
        }
    }

    /// Get the final results of GROUP BY operation - for operations Min, Max and Sum on single GPU
    /// <param name="outKeys">double pointer of output GPU buffer (will be allocated and filled with
    /// final keys)</param> <param name="outValues">double pointer of output GPU buffer (will be
    /// allocated and filled with final values)</param> <param name="outDataElementCount">output CPU
    /// buffer (will be filled with count of reconstructed elements)</param>
    void GetResults(K** outKeys,
                    O** outValues,
                    int32_t* outDataElementCount,
                    int64_t** outKeysNullMask = nullptr,
                    int64_t** outValuesNullMask = nullptr)
    {
        static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
                      "GPUGroupBy.GetResults K (keys) must be integral or floating point");
        static_assert(std::is_integral<V>::value || std::is_floating_point<V>::value,
                      "GPUGroupBy.GetResults V (values) must be integral or floating point");

        // Create buffer for bucket compression - reconstruct
        cuda_ptr<int64_t> occupancyMask(keyBufferSize_, 0);

        // Calculate occupancy mask
        is_bucket_occupied_kernel<<<Context::getInstance().calcGridDim(keyBufferSize_),
                                    Context::getInstance().getBlockDim()>>>(occupancyMask.get(),
                                                                            keys_, keyBufferSize_);

        cuda_ptr<int64_t> keysNullMaskCompressed =
            GPUReconstruct::CompressNullMask(CreateKeyNullMask().get(), keyBufferSize_);

        // Reconstruct the keys
        GPUReconstruct::reconstructColKeep(outKeys, outDataElementCount, keys_, occupancyMask.get(),
                                           keyBufferSize_, outKeysNullMask, keysNullMaskCompressed.get());

        if (!std::is_same<AGG, AggregationFunctions::none>::value)
        {
            // Merge multipied arrays (values and occurrences)
            std::tuple<cuda_ptr<V>, cuda_ptr<int64_t>> mergedArrays =
                MergeMultipliedArrays<AGG, V, USE_VALUES, USE_KEY_OCCURRENCES>(values_, keyOccurrenceCount_,
                                                                               occupancyMask.get(), keyBufferSize_,
                                                                               valueBufferMultiplier_);
            cuda_ptr<V> mergedValues = std::move(std::get<0>(mergedArrays));
            cuda_ptr<int64_t> mergedOccurrences = std::move(std::get<1>(mergedArrays));

            if (USE_VALUES)
            {
                cuda_ptr<int64_t> valuesNullMaskCompressed((NullValues::GetNullBitMaskSize(keyBufferSize_)), 0);
                kernel_compress_null_mask<<<Context::getInstance().calcGridDim(keyBufferSize_),
                                            Context::getInstance().getBlockDim()>>>(
                    reinterpret_cast<int32_t*>(valuesNullMaskCompressed.get()), valuesNullMask_, keyBufferSize_);

                // Reconstruct aggregated values according to the operation
                if (DIRECT_VALUES) // for min, max and sum: mergedValues.get() are direct results, just reconstruct them
                {
                    if (!std::is_same<O, V>::value)
                    {
                        CheckQueryEngineError(GPU_EXTENSION_ERROR,
                                              "Input and output value data type "
                                              "must be the same in GROUP BY");
                    }
                    // reinterpret_cast is needed to solve compilation error
                    GPUReconstruct::reconstructColKeep(outValues, outDataElementCount,
                                                       reinterpret_cast<O*>(mergedValues.get()),
                                                       occupancyMask.get(), keyBufferSize_, outValuesNullMask,
                                                       valuesNullMaskCompressed.get());
                }
                else if (std::is_same<AGG, AggregationFunctions::avg>::value) // for avg: mergedValues.get() need to be divided by keyOccurrences_ and reconstructed
                {
                    cuda_ptr<O> outValuesGPU(keyBufferSize_);
                    // Divide by counts to get averages for buckets
                    try
                    {
                        GPUBinary<ArithmeticOperations::div, O, V*, int64_t*>::Binary(
                            outValuesGPU.get(), mergedValues.get(), mergedOccurrences.get(), keyBufferSize_);
                    }
                    catch (const query_engine_error& err)
                    {
                        // Rethrow just if error is not division by zero.
                        // Division by zero is OK here because it is more efficient to perform division
                        // on raw (not reconstructed) hash table - and some keyOccurrences here can be 0.
                        if (err.GetQueryEngineError() != QueryEngineErrorType::GPU_DIVISION_BY_ZERO_ERROR)
                        {
                            throw err;
                        }
                    }
                    // Reonstruct result with original occupancyMask
                    GPUReconstruct::reconstructColKeep(outValues, outDataElementCount, outValuesGPU.get(),
                                                       occupancyMask.get(), keyBufferSize_, outValuesNullMask,
                                                       valuesNullMaskCompressed.get());
                }
            }
            else if (std::is_same<AGG, AggregationFunctions::count>::value) // for count: reconstruct and return keyOccurrences_
            {
                if (!std::is_same<O, int64_t>::value)
                {
                    CheckQueryEngineError(GPU_EXTENSION_ERROR, "Output value data type in GROUP BY "
                                                               "with COUNT must be int64_t");
                }
                // reinterpret_cast is needed to solve compilation error
                // not reinterpreting anything here actually, outValues is int64_t** always in this else-branch
                GPUReconstruct::reconstructColKeep(reinterpret_cast<int64_t**>(outValues),
                                                   outDataElementCount, mergedOccurrences.get(),
                                                   occupancyMask.get(), keyBufferSize_);
                if (outValuesNullMask)
                {
                    if (*outDataElementCount == 0)
                    {
                        *outValuesNullMask = nullptr;
                    }
                    else
                    {
                        GPUMemory::allocAndSet(outValuesNullMask, 0,
                                               (*outDataElementCount + sizeof(int8_t) * 8 - 1) /
                                                   (sizeof(int8_t) * 8));
                    }
                }
            }
        }
    }

    /// Merge results from all devices and store to buffers on default device (multi GPU function)
    /// <param name="outKeys">double pointer of output GPU buffer (will be allocated and filled with final keys)</param>
    /// <param name="outValues">double pointer of output GPU buffer (will be allocated and filled with final values)</param>
    /// <param name="outDataElementCount">output CPU buffer (will be filled with count of reconstructed elements)</param>
    /// <param name="tables">vector of unique pointers to IGroupBy objects with hash tables on every device (GPU)</param>
    void GetResults(K** outKeys,
                    O** outValues,
                    int32_t* outDataElementCount,
                    std::vector<std::unique_ptr<IGroupBy>>& tables,
                    int64_t** outKeysNullMask = nullptr,
                    int64_t** outValuesNullMask = nullptr)
    {
        if (tables.size() <= 0) // invalid count of tables
        {
            throw std::invalid_argument("Number of tables have to be at least 1.");
        }
        else if (tables.size() == 1 || tables[1].get() == nullptr) // just one table
        {
            GetResults(outKeys, outValues, outDataElementCount, outKeysNullMask, outValuesNullMask);
        }
        else // more tables
        {
            int oldDeviceId = Context::getInstance().getBoundDeviceID();

            std::vector<K> keysAllHost;
            std::vector<int64_t> keysNullMaskAllHost;
            std::vector<V> valuesAllHost;
            std::vector<int64_t> valuesNullMaskAllHost;
            std::vector<int64_t> occurrencesAllHost;
            int32_t sumElementCount = 0;

            // Collect data from all devices (graphic cards) to host
            for (int i = 0; i < tables.size(); i++)
            {
                if (tables[i].get() == nullptr)
                {
                    break;
                }
                // TODO change to cudaMemcpyPeerAsync
                GPUGroupBy<AGG, O, K, V>* table =
                    reinterpret_cast<GPUGroupBy<AGG, O, K, V>*>(tables[i].get());
                std::unique_ptr<K[]> keys = std::make_unique<K[]>(table->GetMaxHashCount());
                std::unique_ptr<int64_t[]> keysNullMask =
                    std::make_unique<int64_t[]>(table->GetMaxHashCount());
                std::unique_ptr<V[]> values = std::make_unique<V[]>(table->GetMaxHashCount());
                std::unique_ptr<int64_t[]> valuesNullMask =
                    std::make_unique<int64_t[]>(table->GetMaxHashCount());
                std::unique_ptr<int64_t[]> occurrences =
                    std::make_unique<int64_t[]>(table->GetMaxHashCount());
                int32_t elementCount;
                Context::getInstance().bindDeviceToContext(i);

                // Reconstruct raw keys with its null mask, values with its null mask (conditional) and occurrences (conditional)
                table->ReconstructRawNumbers(keys.get(), keysNullMask.get(), values.get(),
                                             valuesNullMask.get(), occurrences.get(), &elementCount);

                // Append data to host vectors
                // - keys
                keysAllHost.insert(keysAllHost.end(), keys.get(), keys.get() + elementCount);
                // - null masks as input for finalGroupBy
                keysNullMaskAllHost.insert(keysNullMaskAllHost.end(), keysNullMask.get(),
                                           keysNullMask.get() + elementCount);

                if (USE_VALUES)
                {
                    valuesAllHost.insert(valuesAllHost.end(), values.get(), values.get() + elementCount);
                    valuesNullMaskAllHost.insert(valuesNullMaskAllHost.end(), valuesNullMask.get(),
                                                 valuesNullMask.get() + elementCount);
                }
                if (USE_KEY_OCCURRENCES)
                {
                    occurrencesAllHost.insert(occurrencesAllHost.end(), occurrences.get(),
                                              occurrences.get() + elementCount);
                }
                sumElementCount += elementCount;
            }

            Context::getInstance().bindDeviceToContext(oldDeviceId);
            if (sumElementCount > 0)
            {
                cuda_ptr<K> keysAllGPU(sumElementCount);
                cuda_ptr<int64_t> keysNullMaskAllGPU(sumElementCount);
                cuda_ptr<V> valuesAllGPU(sumElementCount);
                cuda_ptr<int64_t> valuesNullMaskAllGPU(sumElementCount);
                cuda_ptr<int64_t> occurrencesAllGPU(sumElementCount);

                // Copy the condens from host to default device
                GPUMemory::copyHostToDevice(keysAllGPU.get(), keysAllHost.data(), sumElementCount);
                GPUMemory::copyHostToDevice(keysNullMaskAllGPU.get(), keysNullMaskAllHost.data(), sumElementCount);

                if (USE_VALUES)
                {
                    GPUMemory::copyHostToDevice(valuesAllGPU.get(), valuesAllHost.data(), sumElementCount);
                    GPUMemory::copyHostToDevice(valuesNullMaskAllGPU.get(),
                                                valuesNullMaskAllHost.data(), sumElementCount);
                }
                if (USE_KEY_OCCURRENCES)
                {
                    GPUMemory::copyHostToDevice(occurrencesAllGPU.get(), occurrencesAllHost.data(), sumElementCount);
                }

                // Merge results
                if (DIRECT_VALUES) // for min, max and sum
                {
                    GPUGroupBy<AGG, O, K, V> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU.get(), valuesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullMaskAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullMaskAllGPU.get(), sumElementCount)
                            .get());
                    finalGroupBy.GetResults(outKeys, outValues, outDataElementCount,
                                            outKeysNullMask, outValuesNullMask);
                }
                else if (std::is_same<AGG, AggregationFunctions::avg>::value) // for avg
                {
                    V* valuesMerged = nullptr;
                    int64_t* occurrencesMerged = nullptr;

                    // Calculate sum of values
                    // Initialize new empty sumGroupBy table
                    K* tempKeys = nullptr;
                    int64_t* tempKeysNulls;
                    GPUGroupBy<AggregationFunctions::sum, V, K, V> sumGroupBy(sumElementCount, 1);
                    sumGroupBy.ProcessBlock(
                        keysAllGPU.get(), valuesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullMaskAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullMaskAllGPU.get(), sumElementCount)
                            .get());
                    sumGroupBy.GetResults(&tempKeys, &valuesMerged, outDataElementCount, &tempKeysNulls);
                    int64_t firstChar;
                    GPUMemory::copyDeviceToHost(&firstChar, tempKeysNulls, 1);

                    // Calculate sum of occurrences
                    // Initialize countGroupBy table with already existing keys from sumGroupBy - to guarantee the same order
                    GPUGroupBy<AggregationFunctions::sum, int64_t, K, int64_t> countGroupBy(
                        *outDataElementCount, tempKeys, firstChar & 1 == 1);
                    GPUMemory::free(tempKeys);
                    GPUMemory::free(tempKeysNulls);
                    countGroupBy.ProcessBlock(
                        keysAllGPU.get(), occurrencesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullMaskAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullMaskAllGPU.get(), sumElementCount)
                            .get());
                    countGroupBy.GetResults(outKeys, &occurrencesMerged, outDataElementCount,
                                            outKeysNullMask, outValuesNullMask);

                    // Divide merged values by merged occurrences to get final averages
                    GPUMemory::alloc(outValues, *outDataElementCount);
                    try
                    {
                        GPUBinary<ArithmeticOperations::div, O, V*, int64_t*>::Binary(*outValues, valuesMerged,
                                                                                      occurrencesMerged,
                                                                                      *outDataElementCount);
                    }
                    catch (const query_engine_error& err)
                    {
                        // Rethrow just if error is not division by zero.
                        // Division by zero is OK here because some values could be NULL
                        // and therefore keyOccurrences could be 0.
                        if (err.GetQueryEngineError() != QueryEngineErrorType::GPU_DIVISION_BY_ZERO_ERROR)
                        {
                            throw err;
                        }
                    }

                    GPUMemory::free(valuesMerged);
                    GPUMemory::free(occurrencesMerged);
                }
                else if (std::is_same<AGG, AggregationFunctions::count>::value) // for count
                {
                    if (!std::is_same<O, int64_t>::value)
                    {
                        CheckQueryEngineError(GPU_EXTENSION_ERROR,
                                              "Output value data type in GROUP BY with COUNT must "
                                              "be int64_t");
                    }
                    GPUGroupBy<AggregationFunctions::sum, int64_t, K, int64_t> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU.get(), occurrencesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullMaskAllGPU.get(), sumElementCount).get(),
                        nullptr);
                    // reinterpret_cast is needed to solve compilation error
                    finalGroupBy.GetResults(outKeys, reinterpret_cast<int64_t**>(outValues),
                                            outDataElementCount, outKeysNullMask, outValuesNullMask);
                }
                else // for group by without agg
                {
                    GPUGroupBy<AGG, O, K, V> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU.get(), nullptr, sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullMaskAllGPU.get(), sumElementCount).get(),
                        nullptr);
                    finalGroupBy.GetResults(outKeys, outValues, outDataElementCount,
                                            outKeysNullMask, outValuesNullMask);
                }
            }
            else
            {
                *outDataElementCount = 0;
            }
        }
    }
};


// Specialization for String keys
template <typename AGG, typename O, typename V>
class GPUGroupBy<AGG, O, std::string, V>;

// Specialization for multi-keys (GROUP BY multiple column)
template <typename AGG, typename O, typename V>
class GPUGroupBy<AGG, O, std::vector<void*>, V>;
