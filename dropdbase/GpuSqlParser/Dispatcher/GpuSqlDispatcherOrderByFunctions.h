#pragma once

#include "../GpuSqlDispatcher.h"
#include "../../QueryEngine/GPUCore/GPUOrderBy.cuh"
#include "../../QueryEngine/OrderByType.h"

template<typename T>
int32_t GpuSqlDispatcher::orderByCol()
{
	auto colName = arguments.read<std::string>();
	OrderBy::Order order = static_cast<OrderBy::Order>(arguments.read<int32_t>());

	int32_t loadFlag = loadCol<T>(colName);
	if (loadFlag)
	{
		return loadFlag;
	}

	
	if (usingGroupBy)
	{
		if (isOverallLastBlock)
		{
			std::tuple<uintptr_t, int32_t, bool> column = allocatedPointers.at(getAllocatedRegisterName(colName) + (groupByColumns.find(colName) != groupByColumns.end() ? "_keys" : ""));
			int32_t retSize = std::get<1>(column);

			if (orderByTable == nullptr)
			{
				orderByTable = std::make_unique<GPUOrderBy>(retSize);
				int32_t* orderByIndices = allocateRegister<int32_t>("$orderByIndices", retSize);
				usingOrderBy = true;
			}

			std::tuple<uintptr_t, int32_t, bool> orderByIndices = allocatedPointers.at("$orderByIndices");
			orderByTable->OrderByColumn(
				reinterpret_cast<int32_t*>(std::get<0>(orderByIndices)),
				reinterpret_cast<T*>(std::get<0>(column)),
				retSize,
				order);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		std::tuple<uintptr_t, int32_t, bool> column = allocatedPointers.at(getAllocatedRegisterName(colName));
		int32_t retSize = std::get<1>(column);

		if (orderByTable == nullptr)
		{
			orderByTable = std::make_unique<GPUOrderBy>(retSize);
			int32_t* orderByIndices = allocateRegister<int32_t>("$orderByIndices", retSize);
			usingOrderBy = true;
		}

		std::tuple<uintptr_t, int32_t, bool> orderByIndices = allocatedPointers.at("$orderByIndices");
		orderByTable->OrderByColumn(
			reinterpret_cast<int32_t*>(std::get<0>(orderByIndices)),
			reinterpret_cast<T*>(std::get<0>(column)),
			retSize,
			order);
	}	

	return 0;
}

template<typename T>
int32_t GpuSqlDispatcher::orderByConst()
{
	return 0;
}