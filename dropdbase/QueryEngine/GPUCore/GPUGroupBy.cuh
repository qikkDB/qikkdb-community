#pragma once

#include <cstdint>
#include <limits>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include "../Context.h"
#include "GPUMemory.cuh"
#include "GPUArithmetic.cuh"
#include "GPUReconstruct.cuh"

#include "ErrorFlagSwapper.h"
#include "cuda_ptr.h"
#include "IGroupBy.h"
#include "AggregationFunctions.cuh"


// Universal null key calculator
template<typename K>
__device__ __host__ constexpr K getEmptyValue()
{
	static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
		"Unsupported data type of key (in function getEmptyValue)");

	if (std::is_integral<K>::value)
	{
		return std::numeric_limits<K>::min();
	}
	else if (std::is_floating_point<K>::value)
	{
		return std::numeric_limits<K>::quiet_NaN();
	}
}

template<typename T>
__device__ T genericAtomicCAS(T * address, T compare, T val)
{
	static_assert(sizeof(T) == 8 || sizeof(T) == 4, "genericAtomicCAS is working only for 4 Bytes and 8 Bytes long data types");
	if (sizeof(T) == 8)
	{
		uint64_t old = atomicCAS(reinterpret_cast<uint64_t*>(address), *(reinterpret_cast<uint64_t*>(&compare)), *(reinterpret_cast<uint64_t*>(&val)));
		return *(reinterpret_cast<T*>(&old));
	}
	else if (sizeof(T) == 4)
	{
		int32_t old = atomicCAS(reinterpret_cast<int32_t*>(address), *(reinterpret_cast<int32_t*>(&compare)), *(reinterpret_cast<int32_t*>(&val)));
		return *(reinterpret_cast<T*>(&old));
	}
	else
	{
		return T{ 0 };
	}
}


// Kernel
template<typename AGG, typename K, typename V>
__global__ void group_by_kernel(
	K *keys,
	V *values,
	int64_t *keyOccurenceCount,
	int32_t maxHashCount,
	K *inKeys,
	V *inValues,
	int32_t dataElementCount,
	int32_t *errorFlag) {

	const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
	const int32_t stride = blockDim.x * gridDim.x;

	for (int32_t i = idx; i < dataElementCount; i += stride)
	{
		int32_t hash = static_cast<int32_t>(inKeys[i]); // TODO maybe improve hashing for float
		int32_t foundIndex = -1;
		for (int32_t j = 0; j < maxHashCount; j++) {
			// Calculate hash - use type conversion because of float
			int32_t index = abs((hash + j) % maxHashCount);

			//Check if key is not empty and key is not equal to the currently inserted key
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
					continue;  // Try to find another index
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
						printf("this will never happen: %d\n", inKeys[i]);
						continue; // try to find another index
					}
				}*/
			}
			else if (keys[index] != inKeys[i])
			{
				continue;  // try to find another index
			}

			//The key was added or found as already existing
			foundIndex = index;
			break;
		}

		// If no index was found - the hash table is full 
		// else if we found a valid index
		if (foundIndex == -1)
		{
			atomicExch(errorFlag, static_cast<int32_t>(QueryEngineError::GPU_HASH_TABLE_FULL));
		}
		else
		{
			// Use aggregation of values on the bucket and the corresponding counter
			AGG{}(&values[foundIndex], inValues[i]);
			atomicAdd(reinterpret_cast<uint64_t*>(&keyOccurenceCount[foundIndex]), 1);
		}
	}
}

template<typename K>
__global__ void is_bucket_occupied_kernel(int32_t *occupancyMask, K *keys, int32_t maxHashCount)
{
	const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
	const int32_t stride = blockDim.x * gridDim.x;

	for (int32_t i = idx; i < maxHashCount; i += stride)
	{
		if (keys[i] == getEmptyValue<K>())
		{
			occupancyMask[i] = 0;
		}
		else
		{
			occupancyMask[i] = 1;
		}
	}
}

template<typename AGG, typename O, typename K, typename V>
class GPUGroupBy : public IGroupBy
{
private:
	K *keys_;							// Keys
	V *values_;							// Values
	int64_t *keyOccurenceCount_;		// Count of occurrances of keys		

	int32_t maxHashCount_;				// Maximum size of the result hash table

	ErrorFlagSwapper errorFlagSwapper_;

public:
	// Constructor
	// Allocates hash table of element count: hashHashCount
	GPUGroupBy(int32_t maxHashCount) :
		maxHashCount_(maxHashCount)
	{
		GPUMemory::alloc(&keys_, maxHashCount_);
		GPUMemory::alloc(&values_, maxHashCount_);
		GPUMemory::allocAndSet(&keyOccurenceCount_, 0, maxHashCount_);

		GPUMemory::fillArray(keys_, getEmptyValue<K>(), maxHashCount_);
		GPUMemory::fillArray(values_, AGG::template getInitValue<V>(), maxHashCount_);
	}

	// Destructor
	~GPUGroupBy()
	{
		GPUMemory::free(keys_);
		GPUMemory::free(values_);
		GPUMemory::free(keyOccurenceCount_);
	}

	GPUGroupBy(const GPUGroupBy &) = delete;
	GPUGroupBy& operator=(const GPUGroupBy &) = delete;

	// Group By - callable on the blocks of the input dataset
	void groupBy(K *inKeys, V *inValues, int32_t dataElementCount)
	{
		group_by_kernel <AGG> << <  Context::getInstance().calcGridDim(dataElementCount), Context::getInstance().getBlockDim() >> >
			(keys_, values_, keyOccurenceCount_, maxHashCount_, inKeys, inValues, dataElementCount, errorFlagSwapper_.getFlagPointer());
	}

	// Get the final hash table results - for operations Min, Max and Sum
	void getResults(K *outKeys, O *outValues, int32_t *outDataElementCount)
	{
		static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
			"GPUGroupBy<min/max/sum>.getResults K (keys) must be integral or floating point");
		static_assert(std::is_integral<V>::value || std::is_floating_point<V>::value,
			"GPUGroupBy<min/max/sum>.getResults V (values) must be integral or floating point");
		static_assert(std::is_same<O, V>::value,
			"GPUGroupBy<min/max/sum>.getResults O (outValue) and V (value) must be of the same type (for Min/Max/Sum)");

		// Create buffer for bucket compression - reconstruct
		cuda_ptr<int32_t> occupancyMask(maxHashCount_, 0);

		// Calculate occupancy mask
		is_bucket_occupied_kernel << <  Context::getInstance().calcGridDim(maxHashCount_), Context::getInstance().getBlockDim() >> >
			(occupancyMask.get(), keys_, maxHashCount_);

		// Reconstruct the output
		// Copy back the results based on the operation
		GPUReconstruct::reconstructColKeep(outKeys, outDataElementCount, keys_, occupancyMask.get(), maxHashCount_);
		GPUReconstruct::reconstructColKeep(outValues, outDataElementCount, values_, occupancyMask.get(), maxHashCount_);
	}
};

template<typename O, typename K, typename V>
class GPUGroupBy<AggregationFunctions::avg, O, K, V> : public IGroupBy
{
private:
	K *keys_;							// Keys
	V *values_;							// Values
	int64_t *keyOccurenceCount_;		// Count of occurrances of keys		

	int32_t maxHashCount_;				// Maximum size of the result hash table

	ErrorFlagSwapper errorFlagSwapper_;

public:
	// Constructor
	// Allocates hash table of element count: hashHashCount
	GPUGroupBy(int32_t maxHashCount) :
		maxHashCount_(maxHashCount)
	{
		GPUMemory::alloc(&keys_, maxHashCount_);
		GPUMemory::alloc(&values_, maxHashCount_);
		GPUMemory::allocAndSet(&keyOccurenceCount_, 0, maxHashCount_);

		GPUMemory::fillArray(keys_, getEmptyValue<K>(), maxHashCount_);
		GPUMemory::fillArray(values_, AggregationFunctions::avg::template getInitValue<V>(), maxHashCount_);
	}

	// Destructor
	~GPUGroupBy()
	{
		GPUMemory::free(keys_);
		GPUMemory::free(values_);
		GPUMemory::free(keyOccurenceCount_);
	}

	GPUGroupBy(const GPUGroupBy &) = delete;
	GPUGroupBy& operator=(const GPUGroupBy &) = delete;

	// Group By - callable on the blocks of the input dataset
	void groupBy(K *inKeys, V *inValues, int32_t dataElementCount)
	{
		group_by_kernel <AggregationFunctions::avg> << <  Context::getInstance().calcGridDim(dataElementCount), Context::getInstance().getBlockDim() >> >
			(keys_, values_, keyOccurenceCount_, maxHashCount_, inKeys, inValues, dataElementCount, errorFlagSwapper_.getFlagPointer());
	}

	// Get the final hash table results - for operation Average
	void getResults(K *outKeys, O *outValues, int32_t *outDataElementCount)
	{
		static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
			"GPUGroupBy<avg>.getResults K (keys) must be integral or floating point");
		static_assert(std::is_integral<V>::value || std::is_floating_point<V>::value,
			"GPUGroupBy<avg>.getResults V (values) must be integral or floating point");
		// TODO uncomment
		//static_assert(std::is_floating_point<O>::value,
		//	"GPUGroupBy<avg>.getResults O (outValue) must be floating point for Average operation");

		// Create buffer for bucket compression - reconstruct
		cuda_ptr<int32_t> occupancyMask(maxHashCount_, 0);

		// Calculate occupancy mask
		is_bucket_occupied_kernel << <  Context::getInstance().calcGridDim(maxHashCount_), Context::getInstance().getBlockDim() >> >
			(occupancyMask.get(), keys_, maxHashCount_);

		// TODO maybe somewhen optimize if O and V is the same data type - dont copy values
		//      but it requires one more GPUGroupBy specialization
		/*
		if (std::is_same<O, V>::value) {
			GPUArithmetic::colCol<ArithmeticOperations::div>(values_, values_, keyOccurenceCount_, maxHashCount_);
			GPUReconstruct::reconstructColKeep(outValues, outDataElementCount, values_, occupancyMask.get(), maxHashCount_);
		}
		else
		{
		*/
		cuda_ptr<O> outValuesGPU(maxHashCount_);
		// Divide by counts to get averages for buckets
		GPUArithmetic::colCol<ArithmeticOperations::div>(outValuesGPU.get(), values_, keyOccurenceCount_, maxHashCount_);
		// Reonstruct result with original occupancyMask
		GPUReconstruct::reconstructColKeep(outValues, outDataElementCount, outValuesGPU.get(), occupancyMask.get(), maxHashCount_);
		/*
		}
		*/
		GPUReconstruct::reconstructColKeep(outKeys, outDataElementCount, keys_, occupancyMask.get(), maxHashCount_);
	}
};

template<typename K, typename V>
class GPUGroupBy<AggregationFunctions::count, int64_t, K, V> : public IGroupBy
{
private:
	K *keys_;							// Keys
	V *values_;							// Values
	int64_t *keyOccurenceCount_;		// Count of occurrances of keys		

	int32_t maxHashCount_;				// Maximum size of the result hash table

	ErrorFlagSwapper errorFlagSwapper_;

public:
	// Constructor
	// Allocates hash table of element count: hashHashCount
	GPUGroupBy(int32_t maxHashCount) :
		maxHashCount_(maxHashCount)
	{
		GPUMemory::alloc(&keys_, maxHashCount_);
		GPUMemory::alloc(&values_, maxHashCount_);
		GPUMemory::allocAndSet(&keyOccurenceCount_, 0, maxHashCount_);

		GPUMemory::fillArray(keys_, getEmptyValue<K>(), maxHashCount_);
		GPUMemory::fillArray(values_, AggregationFunctions::count::template getInitValue<V>(), maxHashCount_);
	}

	// Destructor
	~GPUGroupBy()
	{
		GPUMemory::free(keys_);
		GPUMemory::free(values_);
		GPUMemory::free(keyOccurenceCount_);
	}

	GPUGroupBy(const GPUGroupBy &) = delete;
	GPUGroupBy& operator=(const GPUGroupBy &) = delete;

	// Group By - callable on the blocks of the input dataset
	void groupBy(K *inKeys, V *inValues, int32_t dataElementCount)
	{
		group_by_kernel <AggregationFunctions::count> << <  Context::getInstance().calcGridDim(dataElementCount), Context::getInstance().getBlockDim() >> >
			(keys_, values_, keyOccurenceCount_, maxHashCount_, inKeys, inValues, dataElementCount, errorFlagSwapper_.getFlagPointer());
	}

	// Get the final hash table results - for operation Count
	void getResults(K *outKeys, int64_t *outValues, int32_t *outDataElementCount)
	{
		static_assert(std::is_integral<K>::value || std::is_floating_point<K>::value,
			"GPUGroupBy<count>.getResults K (keys) must be integral or floating point");
		static_assert(std::is_integral<V>::value || std::is_floating_point<V>::value,
			"GPUGroupBy<count>.getResults V (values) must be integral or floating point");

		// Create buffer for bucket compression - reconstruct
		cuda_ptr<int32_t> occupancyMask(maxHashCount_, 0);

		// Calculate occupancy mask
		is_bucket_occupied_kernel << <  Context::getInstance().calcGridDim(maxHashCount_), Context::getInstance().getBlockDim() >> >
			(occupancyMask.get(), keys_, maxHashCount_);

		// Reconstruct the output
		GPUReconstruct::reconstructColKeep(outKeys, outDataElementCount, keys_, occupancyMask.get(), maxHashCount_);
		GPUReconstruct::reconstructColKeep(outValues, outDataElementCount, keyOccurenceCount_, occupancyMask.get(), maxHashCount_);
	}
};
