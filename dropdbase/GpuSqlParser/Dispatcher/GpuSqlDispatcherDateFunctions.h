#pragma once
#include "../GpuSqlDispatcher.h"
#include "../../QueryEngine/GPUCore/GPUDate.cuh"
#include "../../QueryEngine/GPUCore/GPUMemory.cuh"
#include "../../QueryEngine/GPUCore/GPUReconstruct.cuh"


template<typename OP>
int32_t GpuSqlDispatcher::dateExtractCol()
{
	auto colName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	int32_t loadFlag = loadCol<int64_t>(colName);
	if (loadFlag)
	{
		return loadFlag;
	}

	std::cout << "ExtractDatePartCol: " << colName << " " << reg << std::endl;

	std::tuple<uintptr_t, int32_t, bool> column = allocatedPointers.at(colName);
	int32_t retSize = std::get<1>(column);

	if (!isRegisterAllocated(reg))
	{
		int32_t * result = allocateRegister<int32_t>(reg, retSize);
		GPUDate::extractCol<OP>(result, reinterpret_cast<int64_t*>(std::get<0>(column)), retSize);
	}

	freeColumnIfRegister<int64_t>(colName);
	return 0;
}

template<typename OP>
int32_t GpuSqlDispatcher::dateExtractConst()
{
	int64_t cnst = arguments.read<int64_t>();
	auto reg = arguments.read<std::string>();
	std::cout << "ExtractDatePartConst: " << cnst << " " << reg << std::endl;

	int32_t retSize = 1;

	if (!isRegisterAllocated(reg))
	{
		int32_t * result = allocateRegister<int32_t>(reg, retSize);
		GPUDate::extractConst<OP>(result, cnst, retSize);
	}
	return 0;
}
