#pragma once

#include "CompressionType.h"
#include <vector>
#include "GPUCompression.h"
#include "../DataType.h"

class Compression
{
public:
	
	template<class T>
	static void Compress(DataType columnType, T* const host_uncompressed, int64_t size, std::vector<T>& host_compressed, bool& compressedSuccessfully)
	{
		int64_t compressed_size;
		
		if (columnType == COLUMN_INT || columnType == COLUMN_LONG || columnType == COLUMN_INT8_T)
		{
			compressedSuccessfully = CompressionGPU::compressDataAAFL(host_uncompressed, size, host_compressed, compressed_size);
		}
		else
		{
			compressedSuccessfully = false;
		}
		
	}

	template<class T>
	static void Decompress(DataType columnType, T* const host_compressed, int64_t compressed_size, std::vector<T>& host_uncompressed, bool& compressedSuccessfully)
	{
		if (columnType == COLUMN_INT || columnType == COLUMN_LONG || columnType == COLUMN_INT8_T)
		{
			compressedSuccessfully = CompressionGPU::decompressDataAAFL(host_compressed, compressed_size, host_uncompressed);
		}
		else
		{
			compressedSuccessfully = false;
		}

	}
};