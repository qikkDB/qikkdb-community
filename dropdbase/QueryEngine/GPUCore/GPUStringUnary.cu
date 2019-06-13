#include "GPUStringUnary.cuh"


__global__ void kernel_reverse_string(GPUMemory::GPUString outCol, GPUMemory::GPUString inCol, int64_t stringCount)
{
	const int32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
	const int32_t stride = blockDim.x * gridDim.x;

	for (int32_t i = idx; i < stringCount; i += stride)
	{
		const int64_t firstCharIndex = (i == 0) ? 0 : inCol.stringIndices[i - 1];
		const int64_t length = inCol.stringIndices[i] - firstCharIndex;
		const int64_t lastCharIndex = inCol.stringIndices[i] - 1;

		for (int32_t j = 0; j < length; j++)
		{
			outCol.allChars[firstCharIndex + j] = inCol.allChars[lastCharIndex - j];
		}
	}
}
