#pragma once

#include "../ParserExceptions.h"
#include "../GpuSqlDispatcher.h"
#include "../../QueryEngine/GPUCore/GPUMemory.cuh"
#include "../../QueryEngine/GPUCore/GPUStringUnary.cuh"
#include "../../QueryEngine/GPUCore/GPUStringBinary.cuh"

template<typename OP, typename T>
int32_t GpuSqlDispatcher::stringUnaryCol()
{
	auto colName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	int32_t loadFlag = loadCol<T>(colName);
	if (loadFlag)
	{
		return loadFlag;
	}
	
	std::cout << "StringUnaryCol: " << colName << " " << reg << std::endl;

	if (groupByColumns.find(colName) != groupByColumns.end())
	{
		throw StringGroupByException();
	}
	else if (isLastBlockOfDevice || !usingGroupBy)
	{
		auto column = findStringColumn(colName);
		int32_t retSize = std::get<1>(column);

		if (!isRegisterAllocated(reg))
		{
			GPUMemory::GPUString result;
			GPUStringUnary::Col<OP>(result, std::get<0>(column), retSize);
			fillStringRegister(result, reg, retSize);
		}
	}
	return 0;
}

template<typename OP, typename T>
int32_t GpuSqlDispatcher::stringUnaryConst()
{
	T cnst = arguments.read<T>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringUnaryConst: " << reg << std::endl;

	GPUMemory::GPUString gpuString = insertConstStringGpu(cnst);
	int32_t retSize = 1;

	if (!isRegisterAllocated(reg))
	{
		GPUMemory::GPUString result;
		GPUStringUnary::Const<OP>(result, gpuString, retSize);
		fillStringRegister(result, reg, retSize);
	}
	return 0;
}

template<typename OP, typename T>
int32_t GpuSqlDispatcher::stringIntUnaryCol()
{
	auto colName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	int32_t loadFlag = loadCol<T>(colName);
	if (loadFlag)
	{
		return loadFlag;
	}

	std::cout << "StringIntUnaryCol: " << colName << " " << reg << std::endl;

	if (groupByColumns.find(colName) != groupByColumns.end())
	{
		throw StringGroupByException();
	}
	else if (isLastBlockOfDevice || !usingGroupBy)
	{
		auto column = findStringColumn(colName);
		int32_t retSize = std::get<1>(column);

		if (!isRegisterAllocated(reg))
		{
			int32_t* result = allocateRegister<int32_t>(reg, retSize);
			GPUStringUnary::Col<OP>(result, std::get<0>(column), retSize);
		}
	}
	return 0;
}

template<typename OP, typename T>
int32_t GpuSqlDispatcher::stringIntUnaryConst()
{
	T cnst = arguments.read<T>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringUnaryConst: " << reg << std::endl;

	GPUMemory::GPUString gpuString = insertConstStringGpu(cnst);
	int32_t retSize = 1;

	if (!isRegisterAllocated(reg))
	{
		int32_t* result = allocateRegister<int32_t>(reg, retSize);
		GPUStringUnary::Const<OP>(result, gpuString, retSize);
	}
	return 0;
}


template<typename OP, typename T, typename U>
int32_t GpuSqlDispatcher::stringBinaryColCol()
{
	auto colNameRight = arguments.read<std::string>();
	auto colNameLeft = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringBinaryColCol: " << reg << std::endl;

	return 0;
}

template<typename OP, typename T, typename U>
int32_t GpuSqlDispatcher::stringBinaryColConst()
{
	U cnst = arguments.read<U>();
	auto colName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringBinaryColConst: " << reg << std::endl;

	return 0;
}

template<typename OP, typename T, typename U>
int32_t GpuSqlDispatcher::stringBinaryConstCol()
{
	auto colName = arguments.read<std::string>();
	T cnst = arguments.read<T>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringBinaryConstCol: " << reg << std::endl;

	return 0;
}

template<typename OP, typename T, typename U>
int32_t GpuSqlDispatcher::stringBinaryConstConst()
{
	U cnstRight = arguments.read<U>();
	T cnstLeft = arguments.read<T>();
	auto reg = arguments.read<std::string>();

	std::cout << "StringBinaryConstConst: " << reg << std::endl;

	return 0;
}