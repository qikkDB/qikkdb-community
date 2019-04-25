#pragma once

#include <cstdint>

#include <cuda.h>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdexcept>

#include "../Context.h"
#include "../CudaMemAllocator.h"
#include "../../NativeGeoPoint.h"


template<typename T>
__global__ void kernel_fill_array(T *p_Block, T value, size_t dataElementCount)
{
	const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
	const int32_t stride = blockDim.x * gridDim.x;

	for (int32_t i = idx; i < dataElementCount; i += stride)
	{
		p_Block[i] = value;
	}
}

// Memory methods
class GPUMemory {
public:

	/// <summary>
	/// A structure to represent a complex polygon
	/// </summary>
	struct GPUPolygon
	{
		// Points of polygons
		NativeGeoPoint* polyPoints;
		// Start indexes of each polygon in point array
		int32_t* pointIdx;
		// Number of points of each polygon
		int32_t* pointCount;
		// Start indexes of each complex polygon in polygon array
		int32_t* polyIdx;
		// Number of polygons of each complex polygon
		int32_t* polyCount;
	};

	static bool EvictWithLockList();

	// Memory allocation
	/// <summary>
	/// Memory allocation of block on the GPU with the respective size of the input parameter type
	/// </summary>
	/// <param name="p_Block">pointer to pointer wich will points to allocated memory block on the GPU</param>
	/// <param name="dataType">type of the resulting buffer</param>
	/// <param name="size">count of elements in the block</param>
	/// <returns>return code tells if operation was successful (GPU_EXTENSION_SUCCESS)
	/// or some error occured (GPU_EXTENSION_ERROR)</returns>
	template<typename T>
	static void alloc(T **p_Block, size_t dataElementCount)
	{
		bool allocOK = false;
		while (!allocOK)
		{
			try
			{
				*p_Block = reinterpret_cast<T*>(Context::getInstance().GetAllocatorForCurrentDevice().allocate(dataElementCount * sizeof(T)));
				allocOK = true;
			}
			catch (const std::out_of_range& e)
			{
				if (!EvictWithLockList())
				{
					std::rethrow_exception(std::current_exception());
				}
			}
		}
		CheckCudaError(cudaGetLastError());
	}

	// malloc + memset
	/// <summary>
	/// Memory allocation of block on the GPU with the respective size of the input parameter type
	/// </summary>
	/// <param name="p_Block">pointer to pointer wich will points to allocated memory block on the GPU</param>
	/// <param name="dataType">type of the resulting buffer</param>
	/// <param name="size">count of elements in the block</param>
	/// <param name="value">value to set the memory to
	/// (always has to be int, because of cudaMemset; and just lowest byte will be used
	/// and all bytes in the allocated buffer will be set to that byte value)</param>
	/// <returns>return code tells if operation was successful (GPU_EXTENSION_SUCCESS)
	/// or some error occured (GPU_EXTENSION_ERROR)</returns>
	template<typename T>
	static void allocAndSet(T **p_Block, int value, size_t dataElementCount)
	{
		*p_Block = reinterpret_cast<T*>(Context::getInstance().GetAllocatorForCurrentDevice().allocate(dataElementCount * sizeof(T)));

		memset(*p_Block, value, dataElementCount);

		CheckCudaError(cudaGetLastError());
	}

	// Fill an array with a desired value
	template<typename T>
	static void memset(T *p_Block, int value, size_t dataElementCount)
	{
		//cudaMemsetAsync(p_Block, value, dataElementCount * sizeof(T));	// Async version, uncomment if needed
		cudaMemsetAsync(p_Block, value, dataElementCount * sizeof(T));
		CheckCudaError(cudaGetLastError());
	}
	#ifdef __CUDACC__
	template<typename T>
	static void fillArray(T *p_Block, T value, size_t dataElementCount)
	{
		kernel_fill_array << < Context::getInstance().calcGridDim(dataElementCount), Context::getInstance().getBlockDim() >> >
			(p_Block, value, dataElementCount);

		CheckCudaError(cudaGetLastError());
	}
	#endif

	// Moving data from host to device
		/// <summary>
	/// Copy memory block with dataType numbers from host (RAM, CPU's memory) to device (GPU's memory).
	/// </summary>
	/// <param name="p_BlockDevice">pointer to memory block on device</param>
	/// <param name="p_BlockHost">pointer to memory block on host</param>
	/// <param name="dataType">type of the elements buffer</param>
	/// <param name="size">count of int8_t numbers</param>
	/// <returns>return code tells if operation was successful (GPU_EXTENSION_SUCCESS)
	/// or some error occured (GPU_EXTENSION_ERROR)</returns>
	template<typename T>
	static void copyHostToDevice(T *p_BlockDevice, T *p_BlockHost, size_t dataElementCount)
	{
		cudaMemcpyAsync(p_BlockDevice, p_BlockHost, dataElementCount * sizeof(T), cudaMemcpyHostToDevice);
		CheckCudaError(cudaGetLastError());
	}

	// Moving data from device to host
		/// <summary>
	/// Copy memory block with dataType numbers from device (GPU's memory) to host (RAM, CPU's memory).
	/// </summary>
	/// <param name="p_BlockHost">pointer to memory block on host</param>
	/// <param name="p_BlockDevice">pointer to memory block on device</param>
	/// <param name="dataType">type of the elements buffer</param>
	/// <param name="size">count of int8_t numbers</param>
	/// <returns>return code tells if operation was successful (GPU_EXTENSION_SUCCESS)
	/// or some error occured (GPU_EXTENSION_ERROR)</returns>
	template<typename T>
	static void copyDeviceToHost(T *p_BlockHost, T *p_BlockDevice, size_t dataElementCount)
	{
		cudaMemcpy(p_BlockHost, p_BlockDevice, dataElementCount * sizeof(T), cudaMemcpyDeviceToHost);
		CheckCudaError(cudaGetLastError());
	}

	template<typename T>
	static void copyDeviceToDevice(T *p_BlockDestination, T *p_BlockSource, size_t dataElementCount)
	{
		cudaMemcpy(p_BlockDestination, p_BlockSource, dataElementCount * sizeof(T), cudaMemcpyDeviceToDevice);
		CheckCudaError(cudaGetLastError());
	}

	// Freeing data
		/// <summary>
	/// Free memory block from GPU's memory
	/// </summary>
	/// <param name="p_Block">pointer to memory block (on GPU memory)</param>
	/// <returns>return code tells if operation was successful (GPU_EXTENSION_SUCCESS)
	/// or some error occured (GPU_EXTENSION_ERROR)</returns>
	static void free(void *p_block)
	{
		Context::getInstance().GetAllocatorForCurrentDevice().deallocate(static_cast<int8_t*>(p_block), 0);
		CheckCudaError(cudaGetLastError());
	}

	template<typename T>
	static void hostRegister(T **devicePtr, T *hostPtr, size_t dataElementCount)
	{
		cudaHostRegister(hostPtr, dataElementCount * sizeof(T), cudaHostRegisterMapped);
		cudaHostGetDevicePointer(devicePtr, hostPtr, 0);

		CheckCudaError(cudaGetLastError());
	}

	template<typename T>
	static void hostUnregister(T *hostPtr)
	{
		cudaHostUnregister(hostPtr);
		CheckCudaError(cudaGetLastError());
	}

	// Pin host memory
	template<typename T>
	static void hostPin(T* hostPtr, size_t dataElementCount)
	{
		cudaHostRegister(hostPtr, dataElementCount * sizeof(T), cudaHostRegisterDefault);
		CheckCudaError(cudaGetLastError());
	}

	// Wipe all allocated memory O(1)
	static void clear()
	{
		Context::getInstance().GetAllocatorForCurrentDevice().Clear();
		CheckCudaError(cudaGetLastError());
	}
};
