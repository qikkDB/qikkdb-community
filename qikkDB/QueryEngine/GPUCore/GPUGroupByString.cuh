#pragma once

#include "GPUFilter.cuh"
#include "GPUFilterConditions.cuh"
#include "GPUGroupBy.cuh"
#include "GPUStringUnary.cuh"


constexpr int32_t GBS_SOURCE_INDEX_EMPTY_KEY = -1;
constexpr int32_t GBS_SOURCE_INDEX_KEY_IN_BUFFER = -2;
constexpr int32_t GBS_STRING_HASH_COEF = 31;


__device__ int32_t GetHash(char* text, int32_t length);


__device__ bool AreEqualStrings(char* textA, int32_t lenghtA, GPUMemory::GPUString stringColB, int64_t indexB);


__device__ bool IsNewKey(char* checkedKeyChars,
                         int32_t checkedKeyLength,
                         GPUMemory::GPUString inKeys,
                         GPUMemory::GPUString keysBuffer,
                         int32_t* sourceIndices,
                         int32_t index);


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
template <typename AGG, typename V>
__global__ void kernel_group_by_string(int32_t* sourceIndices,
                                       int32_t* stringLengths,
                                       GPUMemory::GPUString keysBuffer,
                                       V* values,
                                       nullarray_t* valuesNullArrayUncompressed,
                                       int64_t* keyOccurrenceCount,
                                       const int32_t loweredMaxHashCount,
                                       GPUMemory::GPUString inKeys,
                                       V* inValues,
                                       const int32_t dataElementCount,
                                       const int32_t arrayMultiplier,
                                       int32_t* errorFlag,
                                       nullmask_t* inKeysNullMask,
                                       nullmask_t* inValuesNullMask)
{
    const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    const int32_t stride = blockDim.x * gridDim.x;

    for (int32_t i = idx; i < dataElementCount &&
                          *errorFlag != static_cast<int32_t>(QueryEngineErrorType::GPU_HASH_TABLE_FULL);
         i += stride)
    {
        const bool nullKey =
            (inKeysNullMask != nullptr) && (NullValues::GetConcreteBitFromBitmask(inKeysNullMask, i));
        const bool nullValue = (inValuesNullMask != nullptr) &&
                               (NullValues::GetConcreteBitFromBitmask(inValuesNullMask, i));
        int32_t foundIndex = -1;
        int32_t inKeyLength = 0;

        if (!nullKey)
        {
            const int64_t inKeyIndex = GetStringIndex(inKeys.stringIndices, i);
            inKeyLength = GetStringLength(inKeys.stringIndices, i);
            char* inKeyChars = inKeys.allChars + inKeyIndex;
            // Calculate hash
            const int32_t hash = abs(GetHash(inKeyChars, inKeyLength)) % loweredMaxHashCount;

            for (int32_t j = 0; j < loweredMaxHashCount; j++)
            {
                // Calculate index to hash-table from hash
                const int32_t index = ((hash + j) % loweredMaxHashCount) + 1;

                // Check if key is not empty and key is not equal to the currently inserted key
                if (sourceIndices[index] != GBS_SOURCE_INDEX_EMPTY_KEY &&
                    IsNewKey(inKeyChars, inKeyLength, inKeys, keysBuffer, sourceIndices, index))
                {
                    continue; // Hash collision - try to find another index
                }

                // If key is empty
                if (sourceIndices[index] == GBS_SOURCE_INDEX_EMPTY_KEY)
                {
                    // Compare key at index with Empty and if equals, store there inKey
                    int32_t old =
                        genericAtomicCAS<int32_t>(&sourceIndices[index], GBS_SOURCE_INDEX_EMPTY_KEY, i);

                    // Check if some other thread stored different key to this index
                    if (old != GBS_SOURCE_INDEX_EMPTY_KEY && old != i &&
                        IsNewKey(inKeyChars, inKeyLength, inKeys, keysBuffer, sourceIndices, index))
                    {
                        continue; // Try to find another index
                    }
                }
                else if (sourceIndices[index] != i &&
                         IsNewKey(inKeyChars, inKeyLength, inKeys, keysBuffer, sourceIndices, index))
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
            sourceIndices[foundIndex] = GBS_SOURCE_INDEX_KEY_IN_BUFFER;
        }

        // If no index was found - the hash table is full
        // else if we found a valid index
        if (foundIndex == -1)
        {
            atomicExch(errorFlag, static_cast<int32_t>(QueryEngineErrorType::GPU_HASH_TABLE_FULL));
        }
        else
        {
            stringLengths[foundIndex] = inKeyLength;
            if (!nullValue)
            {
                // Aggregate value
                if (values)
                {
                    AGG{}(values + foundIndex * arrayMultiplier + threadIdx.x % arrayMultiplier, inValues[i]);
                    if (valuesNullArrayUncompressed[foundIndex])
                    {
                        valuesNullArrayUncompressed[foundIndex] = 0;
                    }
                }
                // Increment occurrence counter
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


__global__ void kernel_collect_string_keys(GPUMemory::GPUString sideBuffer,
                                           int32_t* sourceIndices,
                                           int32_t* stringLengths,
                                           GPUMemory::GPUString keysBuffer,
                                           int32_t maxHashCount,
                                           GPUMemory::GPUString inKeys,
                                           int32_t inKeysCount);


__global__ void kernel_source_indices_to_mask(int8_t* occupancyMask, int32_t* sourceIndices, int32_t maxHashCount);


__global__ void kernel_mark_collected_strings(int32_t* sourceIndices, int32_t maxHashCount);


/// GROUP BY generic class for String keys
template <typename AGG, typename O, typename V>
class GPUGroupBy<AGG, O, std::string, V> : public IGroupBy
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

public:
    /// Temp buffer where one value points to input key
    /// or tells the key is already in keysBuffer_
    int32_t* sourceIndices_ = nullptr;
    /// Buffer with lengths of collected string keys
    int32_t* stringLengths_ = nullptr;
    /// Buffer with collected string keys
    GPUMemory::GPUString keysBuffer_{nullptr, nullptr};

private:
    /// Value buffer of the hash table
    V* values_ = nullptr;
    nullarray_t* valuesNullArrayUncompressed_ = nullptr;

    /// Count of values aggregated per key (helper buffer of the hash table)
    int64_t* keyOccurrenceCount_ = nullptr;

    /// Size of the hash table (max. count of unique keys)
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
            // Allocate buffers needed for key storing
            GPUMemory::alloc(&sourceIndices_, keyBufferSize_);
            GPUMemory::allocAndSet(&stringLengths_, 0, keyBufferSize_);
            // And for values and occurrences
            if (USE_VALUES)
            {
                GPUMemory::alloc(&values_, valueBufferSize);
                GPUMemory::allocAndSet(&valuesNullArrayUncompressed_, 1, keyBufferSize_);
            }
            if (USE_KEY_OCCURRENCES)
            {
                GPUMemory::allocAndSet(&keyOccurrenceCount_, 0, valueBufferSize);
            }
        }
        catch (...)
        {
            if (sourceIndices_)
            {
                GPUMemory::free(sourceIndices_);
            }
            if (stringLengths_)
            {
                GPUMemory::free(stringLengths_);
            }
            if (values_)
            {
                GPUMemory::free(values_);
            }
            if (valuesNullArrayUncompressed_)
            {
                GPUMemory::free(valuesNullArrayUncompressed_);
            }
            if (keyOccurrenceCount_)
            {
                GPUMemory::free(keyOccurrenceCount_);
            }
            throw;
        }
        GPUMemory::fillArray(sourceIndices_, GBS_SOURCE_INDEX_EMPTY_KEY, keyBufferSize_);
        if (USE_VALUES)
        {
            GPUMemory::fillArray(values_, AGG::template getInitValue<V>(), valueBufferSize);
        }
    }

    /// Create GPUGroupBy object with existing keys (allocate whole new hash table)
    /// <param name="maxHashCount">size of the hash table (max. count of unique keys)</param>
    /// <param name="sourceIndices">GPU buffer with original source indices (will be copied to a new buffer)</param>
    /// <param name="stringLengths">GPU buffer with lengths of original string keys (will be copied to a new buffer)</param>
    /// <param name="keysBuffer">GPU buffer with original existing keys (will be copied to a new buffer)</param>
    GPUGroupBy(const int32_t maxHashCount,
               const int32_t keySizeMultiplier,
               int32_t* sourceIndices,
               int32_t* stringLengths,
               GPUMemory::GPUString keysBuffer)
    : GPUGroupBy(maxHashCount, keySizeMultiplier)
    {
        int64_t totalCharCount = 0;
        try
        {
            // Alloc string key buffer
            GPUMemory::alloc(&(keysBuffer_.stringIndices), keyBufferSize_);
            GPUMemory::copyDeviceToHost(&totalCharCount, keysBuffer.stringIndices + keyBufferSize_ - 1, 1);
            if (totalCharCount > 0)
            {
                GPUMemory::alloc(&(keysBuffer_.allChars), totalCharCount);
            }
            else
            {
                keysBuffer_.allChars = nullptr;
            }
        }
        catch (...)
        {
            GPUMemory::free(keysBuffer_);
            throw;
        }
        // Copy string keys
        GPUMemory::copyDeviceToDevice(sourceIndices_, sourceIndices, keyBufferSize_);
        GPUMemory::copyDeviceToDevice(stringLengths_, stringLengths, keyBufferSize_);
        GPUMemory::copyDeviceToDevice(keysBuffer_.stringIndices, keysBuffer.stringIndices, keyBufferSize_);
        if (totalCharCount > 0)
        {
            GPUMemory::copyDeviceToDevice(keysBuffer_.allChars, keysBuffer.allChars, totalCharCount);
        }
    }

    ~GPUGroupBy()
    {
        GPUMemory::free(sourceIndices_);
        GPUMemory::free(stringLengths_);
        GPUMemory::free(keysBuffer_);

        if (USE_VALUES)
        {
            GPUMemory::free(values_);
            GPUMemory::free(valuesNullArrayUncompressed_);
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
    void ProcessBlock(GPUMemory::GPUString inKeys,
                      V* inValues,
                      int32_t dataElementCount,
                      nullmask_t* inKeysNullMask = nullptr,
                      nullmask_t* inValuesNullMask = nullptr)
    {
        if (dataElementCount > 0)
        {
            Context& context = Context::getInstance();
            kernel_group_by_string<AGG><<<context.calcGridDim(dataElementCount), context.getBlockDim()>>>(
                sourceIndices_, stringLengths_, keysBuffer_, values_, valuesNullArrayUncompressed_,
                keyOccurrenceCount_, keyBufferSize_ - 1, inKeys, inValues, dataElementCount,
                valueBufferMultiplier_, errorFlagSwapper_.GetFlagPointer(), inKeysNullMask, inValuesNullMask);
            errorFlagSwapper_.Swap();

            GPUMemory::GPUString sideBuffer;
            try
            {
                GPUMemory::alloc(&(sideBuffer.stringIndices), keyBufferSize_);

                GPUReconstruct::PrefixSum(sideBuffer.stringIndices, stringLengths_, keyBufferSize_);

                int64_t totalCharCount;
                GPUMemory::copyDeviceToHost(&totalCharCount, sideBuffer.stringIndices + keyBufferSize_ - 1, 1);
                GPUMemory::alloc(&(sideBuffer.allChars), totalCharCount);

                kernel_collect_string_keys<<<context.calcGridDim(keyBufferSize_), context.getBlockDim()>>>(
                    sideBuffer, sourceIndices_, stringLengths_, keysBuffer_, keyBufferSize_, inKeys, dataElementCount);

                GPUMemory::free(keysBuffer_);
                keysBuffer_ = sideBuffer;

                kernel_mark_collected_strings<<<context.calcGridDim(keyBufferSize_), context.getBlockDim()>>>(
                    sourceIndices_, keyBufferSize_);
            }
            catch (...)
            {
                GPUMemory::free(sideBuffer);
                throw;
            }
            CheckCudaError(cudaGetLastError());
        }
    }


    /// Get the size of the hash table (max. count of unique keys)
    /// <returns>size of the hash table</returns>
    int32_t GetMaxHashCount()
    {
        return keyBufferSize_;
    }

    /// Create memory-wasting null array for keys - one 1 at [0], other zeros
    cuda_ptr<nullarray_t> CreateKeyNullArray()
    {
        cuda_ptr<nullarray_t> keyNullArray(keyBufferSize_, 0);
        GPUMemory::memset(keyNullArray.get(), 1, 1);
        return keyNullArray;
    }

    /// Reconstruct needed raw fields (do not calculate final results yet)
    /// Reconstruct keys, values and key occurrence counts separately
    /// <param name="keys">output buffer to fill with reconstructed keys</param>
    /// <param name="values">output buffer to fill with reconstructed values</param>
    /// <param name="occurrences">not used buffer if using operations MIN, MAX or SUM - nullptr can be used</param>
    /// <param name="elementCount">ouptut buffer to fill with element count (one int32_t number)</param>
    void ReconstructRawNumbers(std::vector<int32_t>& keysStringLengths,
                               std::vector<char>& keysAllChars,
                               nullarray_t* keysNullArray,
                               V* values,
                               nullarray_t* valuesNullArray,
                               int64_t* occurrences,
                               int32_t* elementCount)
    {
        Context& context = Context::getInstance();
        cuda_ptr<int8_t> occupancyMask(keyBufferSize_);
        kernel_source_indices_to_mask<<<context.calcGridDim(keyBufferSize_), context.getBlockDim()>>>(
            occupancyMask.get(), sourceIndices_, keyBufferSize_);

        cuda_ptr<nullarray_t> keysNullArrayInput = CreateKeyNullArray();

        GPUReconstruct::ReconstructStringColRaw(keysStringLengths, keysAllChars, elementCount,
                                                keysBuffer_, occupancyMask.get(), keyBufferSize_);
        GPUReconstruct::reconstructCol(keysNullArray, elementCount, keysNullArrayInput.get(),
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
            GPUReconstruct::reconstructCol(valuesNullArray, elementCount, valuesNullArrayUncompressed_,
                                           occupancyMask.get(), keyBufferSize_);
        }
        if (USE_KEY_OCCURRENCES)
        {
            GPUReconstruct::reconstructCol(occurrences, elementCount, mergedOccurrences.get(),
                                           occupancyMask.get(), keyBufferSize_);
        }
    }


    /// Get the final results of GROUP BY operation - for operations Min, Max and Sum on single GPU
    /// <param name="outKeys">pointer to GPUString struct (will be allocated and filled with final keys)</param>
    /// <param name="outValues">double pointer of output GPU buffer (will be allocated and filled with final values)</param>
    /// <param name="outDataElementCount">output CPU buffer (will be filled with count of reconstructed elements)</param>
    void GetResults(GPUMemory::GPUString* outKeys,
                    O** outValues,
                    int32_t* outDataElementCount,
                    nullmask_t** outKeysNullMask = nullptr,
                    nullmask_t** outValuesNullMask = nullptr)
    {
        Context& context = Context::getInstance();

        // Create buffer for bucket compression - reconstruction
        cuda_ptr<int8_t> occupancyMask(keyBufferSize_);
        // Compute occupancyMask
        kernel_source_indices_to_mask<<<context.calcGridDim(keyBufferSize_), context.getBlockDim()>>>(
            occupancyMask.get(), sourceIndices_, keyBufferSize_);

        cuda_ptr<nullmask_t> keysNullMaskCompressed =
            GPUReconstruct::CompressNullMask(CreateKeyNullArray().get(), keyBufferSize_);

        GPUReconstruct::ReconstructStringColKeep(outKeys, outDataElementCount, keysBuffer_,
                                                 occupancyMask.get(), keyBufferSize_,
                                                 outKeysNullMask, keysNullMaskCompressed.get());

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
                cuda_ptr<nullmask_t> valuesNullMaskCompressed((NullValues::GetNullBitMaskSize(keyBufferSize_)), 0);

                if (outValuesNullMask)
                {
                    kernel_compress_null_mask<<<Context::getInstance().calcGridDim(keyBufferSize_),
                                                Context::getInstance().getBlockDim()>>>(
                        reinterpret_cast<nullmask_t*>(valuesNullMaskCompressed.get()),
                        valuesNullArrayUncompressed_, keyBufferSize_);
                }

                // Reconstruct aggregated values
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
                    catch (query_engine_error& err)
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
                    CheckQueryEngineError(GPU_EXTENSION_ERROR,
                                          "Output value data type must be int64_t "
                                          "for COUNT-GROUP BY operation");
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
                                               NullValues::GetNullBitMaskSize(*outDataElementCount));
                    }
                }
            }
        }
    }


    /// Merge results from all devices and store to buffers on default device (multi GPU function)
    /// <param name="outKeys">pointer to GPUString struct (will be allocated and filled with final
    /// keys)</param> <param name="outValues">double pointer of output GPU buffer (will be allocated
    /// and filled with final values)</param> <param name="outDataElementCount">output CPU buffer
    /// (will be filled with count of reconstructed elements)</param> <param name="tables">vector of
    /// unique pointers to IGroupBy objects with hash tables on every device (GPU)</param>
    void GetResults(GPUMemory::GPUString* outKeys,
                    O** outValues,
                    int32_t* outDataElementCount,
                    std::vector<std::unique_ptr<IGroupBy>>& tables,
                    nullmask_t** outKeysNullMask = nullptr,
                    nullmask_t** outValuesNullMask = nullptr)
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

            std::vector<int32_t> keysAllHostStringLengths;
            std::vector<char> keysAllHostAllChars;
            std::vector<nullarray_t> keysNullArrayAllHost;

            std::vector<V> valuesAllHost;
            std::vector<nullarray_t> valuesNullArrayAllHost;

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
                GPUGroupBy<AGG, O, std::string, V>* table =
                    reinterpret_cast<GPUGroupBy<AGG, O, std::string, V>*>(tables[i].get());

                std::vector<int32_t> keysStringLengths;
                std::vector<char> keysAllChars;
                std::unique_ptr<nullarray_t[]> keysNullArray =
                    std::make_unique<nullarray_t[]>(table->GetMaxHashCount());

                std::unique_ptr<V[]> values = std::make_unique<V[]>(table->GetMaxHashCount());
                std::unique_ptr<nullarray_t[]> valuesNullArray =
                    std::make_unique<nullarray_t[]>(table->GetMaxHashCount());

                std::unique_ptr<int64_t[]> occurrences =
                    std::make_unique<int64_t[]>(table->GetMaxHashCount());

                int32_t elementCount;
                Context::getInstance().bindDeviceToContext(i);

                // Reconstruct keys and values
                table->ReconstructRawNumbers(keysStringLengths, keysAllChars, keysNullArray.get(),
                                             values.get(), valuesNullArray.get(), occurrences.get(),
                                             &elementCount);

                // Append data to host vectors
                keysAllHostStringLengths.insert(keysAllHostStringLengths.end(),
                                                keysStringLengths.begin(), keysStringLengths.end());
                keysAllHostAllChars.insert(keysAllHostAllChars.end(), keysAllChars.begin(),
                                           keysAllChars.end());

                keysNullArrayAllHost.insert(keysNullArrayAllHost.end(), keysNullArray.get(),
                                            keysNullArray.get() + elementCount);

                if (USE_VALUES)
                {
                    valuesAllHost.insert(valuesAllHost.end(), values.get(), values.get() + elementCount);
                    valuesNullArrayAllHost.insert(valuesNullArrayAllHost.end(), valuesNullArray.get(),
                                                  valuesNullArray.get() + elementCount);
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
                cuda_ptr<int32_t> keysAllGPUStringLengths(sumElementCount);
                cuda_ptr<nullarray_t> keysNullArrayAllGPU(sumElementCount);

                cuda_ptr<V> valuesAllGPU(sumElementCount);
                cuda_ptr<nullarray_t> valuesNullArrayAllGPU(sumElementCount);

                cuda_ptr<int64_t> occurrencesAllGPU(sumElementCount);

                // Copy the condens from host to default device
                GPUMemory::copyHostToDevice(keysAllGPUStringLengths.get(),
                                            keysAllHostStringLengths.data(), sumElementCount);
                GPUMemory::copyHostToDevice(keysNullArrayAllGPU.get(), keysNullArrayAllHost.data(), sumElementCount);

                if (USE_VALUES)
                {
                    GPUMemory::copyHostToDevice(valuesAllGPU.get(), valuesAllHost.data(), sumElementCount);
                    GPUMemory::copyHostToDevice(valuesNullArrayAllGPU.get(),
                                                valuesNullArrayAllHost.data(), sumElementCount);
                }
                if (USE_KEY_OCCURRENCES)
                {
                    GPUMemory::copyHostToDevice(occurrencesAllGPU.get(), occurrencesAllHost.data(), sumElementCount);
                }

                // Construct new GPUString
                GPUMemory::GPUString keysAllGPU;
                GPUMemory::alloc(&(keysAllGPU.stringIndices), sumElementCount);
                GPUReconstruct::PrefixSum(keysAllGPU.stringIndices, keysAllGPUStringLengths.get(), sumElementCount);
                GPUMemory::alloc(&(keysAllGPU.allChars), keysAllHostAllChars.size());
                GPUMemory::copyHostToDevice(keysAllGPU.allChars, keysAllHostAllChars.data(),
                                            keysAllHostAllChars.size());

                // Merge results
                if (DIRECT_VALUES) // for min, max and sum
                {
                    GPUGroupBy<AGG, O, std::string, V> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU, valuesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullArrayAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullArrayAllGPU.get(), sumElementCount)
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
                    GPUMemory::GPUString keysToDiscard;
                    GPUGroupBy<AggregationFunctions::sum, V, std::string, V> sumGroupBy(sumElementCount, 1);
                    sumGroupBy.ProcessBlock(
                        keysAllGPU, valuesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullArrayAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullArrayAllGPU.get(), sumElementCount)
                            .get());
                    sumGroupBy.GetResults(&keysToDiscard, &valuesMerged, outDataElementCount);
                    GPUMemory::free(keysToDiscard);

                    // Calculate sum of occurrences
                    // Initialize countGroupBy table with already existing keys from sumGroupBy - to guarantee the same order
                    GPUGroupBy<AggregationFunctions::sum, int64_t, std::string, int64_t> countGroupBy(
                        sumElementCount, 1, sumGroupBy.sourceIndices_, sumGroupBy.stringLengths_,
                        sumGroupBy.keysBuffer_);
                    countGroupBy.ProcessBlock(
                        keysAllGPU, occurrencesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullArrayAllGPU.get(), sumElementCount).get(),
                        GPUReconstruct::CompressNullMask(valuesNullArrayAllGPU.get(), sumElementCount)
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
                    GPUGroupBy<AggregationFunctions::sum, int64_t, std::string, int64_t> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU, occurrencesAllGPU.get(), sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullArrayAllGPU.get(), sumElementCount).get(),
                        nullptr);
                    // reinterpret_cast is needed to solve compilation error
                    finalGroupBy.GetResults(outKeys, reinterpret_cast<int64_t**>(outValues),
                                            outDataElementCount, outKeysNullMask, outValuesNullMask);
                }

                else // for group by without aggregation function
                {
                    GPUGroupBy<AGG, O, std::string, V> finalGroupBy(sumElementCount, 1);
                    finalGroupBy.ProcessBlock(
                        keysAllGPU, nullptr, sumElementCount,
                        GPUReconstruct::CompressNullMask(keysNullArrayAllGPU.get(), sumElementCount).get(),
                        nullptr);
                    finalGroupBy.GetResults(outKeys, outValues, outDataElementCount,
                                            outKeysNullMask, outValuesNullMask);
                }

                GPUMemory::free(keysAllGPU);
            }
            else
            {
                *outDataElementCount = 0;
            }
        }
    }
};
