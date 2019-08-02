#pragma once

#include "../GpuSqlDispatcher.h"
#include "../../QueryEngine/GPUCore/cuda_ptr.h"
#include "../../QueryEngine/GPUCore/GPUGroupBy.cuh"
#include "../../QueryEngine/GPUCore/GPUGroupByString.cuh"
#include "../../QueryEngine/GPUCore/GPUGroupByMultiKey.cuh"
#include "../../QueryEngine/GPUCore/GPUAggregation.cuh"
#include "../../QueryEngine/GPUCore/GPUReconstruct.cuh"
#include "../../QueryEngine/GPUCore/GPUMemory.cuh"

/// Implementation of generic aggregation operation based on functor OP
/// Used when GROUP BY Clause is not present
/// Loads data on demand
/// COUNT operation is handled more efficiently
/// If WHERE clause is present filtering is done before agreggation
/// <returns name="statusCode">Finish status code of the operation</returns>
template<typename OP, typename OUT, typename IN>
int32_t GpuSqlDispatcher::aggregationCol()
{
	auto colName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	int32_t loadFlag = loadCol<IN>(colName);
	if (loadFlag)
	{
		return loadFlag;
	}

	std::cout << "AggCol: " << colName << " " << reg << std::endl;

	PointerAllocation& column = allocatedPointers.at(colName);
	int32_t reconstructOutSize;

	IN* reconstructOutReg = nullptr;
	if (std::is_same<OP, AggregationFunctions::count>::value)
	{
		// If mask is present - count suitable rows
		if (filter_)
		{
			int32_t *indexes = nullptr;
			GPUReconstruct::GenerateIndexesKeep(&indexes, &reconstructOutSize, reinterpret_cast<int8_t*>(filter_), column.elementCount);
			if (indexes)
			{
				GPUMemory::free(indexes);
			}
		}
		// If mask is nullptr - count full rows
		else {
			reconstructOutSize = column.elementCount;
		}
	}
	else
	{
		GPUReconstruct::reconstructColKeep<IN>(&reconstructOutReg, &reconstructOutSize, reinterpret_cast<IN*>(column.gpuPtr), reinterpret_cast<int8_t*>(filter_), column.elementCount);
	}

	if (column.shouldBeFreed)
	{
		GPUMemory::free(reinterpret_cast<void*>(column.gpuPtr));
	}
	else
	{
		column.shouldBeFreed = true;
	}

	column.gpuPtr = reinterpret_cast<uintptr_t>(reconstructOutReg);
	column.elementCount = reconstructOutSize;

	if (!isRegisterAllocated(reg))
	{
		// TODO: if (not COUNT operation and std::get<1>(column) == 0), set result to NaN
		OUT * result = allocateRegister<OUT>(reg, 1);
		GPUAggregation::col<OP, OUT, IN>(result, reinterpret_cast<IN*>(column.gpuPtr), column.elementCount);
	}
	freeColumnIfRegister<IN>(colName);
	filter_ = 0;
	return 0;
}

template<typename OP, typename OUT, typename IN>
int32_t GpuSqlDispatcher::aggregationConst()
{
	std::cout << "AggConst" << std::endl;
	return 0;
}

template<typename OP, typename O, typename K, typename V>
class GpuSqlDispatcher::GroupByHelper
{
public:
	static std::unique_ptr<IGroupBy> CreateInstance(int32_t groupByBuckets, const std::vector<std::pair<std::string, DataType>>& groupByColumns)
	{
		return std::make_unique<GPUGroupBy<OP, O, K, V>>(Configuration::GetInstance().GetGroupByBuckets());
	}

	static void ProcessBlock(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const PointerAllocation& valueColumn, GpuSqlDispatcher& dispatcher)
	{
		std::string groupByColumnName = groupByColumns.begin()->first;
		PointerAllocation groupByColumn = dispatcher.allocatedPointers.at(groupByColumnName);

		int32_t dataSize = std::min(groupByColumn.elementCount, valueColumn.elementCount);

		reinterpret_cast<GPUGroupBy<OP, O, K, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->ProcessBlock(
			reinterpret_cast<K*>(groupByColumn.gpuPtr), reinterpret_cast<V*>(valueColumn.gpuPtr), dataSize,
			reinterpret_cast<int8_t*>(groupByColumn.gpuNullMaskPtr), reinterpret_cast<int8_t*>(valueColumn.gpuNullMaskPtr));
	}

	static void GetResults(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const std::string& reg, GpuSqlDispatcher& dispatcher)
	{
		std::string groupByColumnName = groupByColumns.begin()->first;
		int32_t outSize;
		K* outKeys = nullptr;
		int8_t* outKeyNullMask = nullptr;
		O* outValues = nullptr;
		int8_t* outValueNullMask = nullptr;
		reinterpret_cast<GPUGroupBy<OP, O, K, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->GetResults(&outKeys, &outValues, &outSize, dispatcher.groupByTables, &outKeyNullMask, &outValueNullMask);
		dispatcher.InsertRegister(groupByColumnName + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(outKeys), outSize, true, reinterpret_cast<uintptr_t>(outKeyNullMask)});
		dispatcher.InsertRegister(reg, PointerAllocation{ reinterpret_cast<uintptr_t>(outValues), outSize, true, reinterpret_cast<uintptr_t>(outValueNullMask)});
	}
};

template<typename OP, typename O, typename V>
class GpuSqlDispatcher::GroupByHelper<OP, O, std::string, V>
{
public:
	static std::unique_ptr<IGroupBy> CreateInstance(int32_t groupByBuckets, const std::vector<std::pair<std::string, DataType>>& groupByColumns)
	{
		return std::make_unique<GPUGroupBy<OP, O, std::string, V>>(Configuration::GetInstance().GetGroupByBuckets());
	}

	static void ProcessBlock(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const PointerAllocation& valueColumn, GpuSqlDispatcher& dispatcher)
	{
		std::string groupByColumnName = groupByColumns.begin()->first;
		auto groupByColumn = dispatcher.findStringColumn(groupByColumnName);

		int32_t dataSize = std::min(std::get<1>(groupByColumn), valueColumn.elementCount);

		reinterpret_cast<GPUGroupBy<OP, O, std::string, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->ProcessBlock(
			std::get<0>(groupByColumn), reinterpret_cast<V*>(valueColumn.gpuPtr), dataSize,
			std::get<2>(groupByColumn), reinterpret_cast<int8_t*>(valueColumn.gpuNullMaskPtr));
	}
	
	static void GetResults(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const std::string& reg, GpuSqlDispatcher& dispatcher)
	{
		std::string groupByColumnName = groupByColumns.begin()->first;
		int32_t outSize;
		GPUMemory::GPUString outKeys;
		int8_t* outKeyNullMask = nullptr;
		O* outValues = nullptr;
		int8_t* outValueNullMask = nullptr;
		reinterpret_cast<GPUGroupBy<OP, O, std::string, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->GetResults(&outKeys, &outValues, &outSize, dispatcher.groupByTables, &outKeyNullMask, &outValueNullMask);
		dispatcher.fillStringRegister(outKeys, groupByColumnName + KEYS_SUFFIX, outSize, true, outKeyNullMask);
		dispatcher.InsertRegister(reg, PointerAllocation{reinterpret_cast<uintptr_t>(outValues), outSize, true, reinterpret_cast<uintptr_t>(outValueNullMask)});
	}
};

template<typename OP, typename O, typename V>
class GpuSqlDispatcher::GroupByHelper<OP, O, std::vector<void*>, V>
{
public:
	static std::unique_ptr<IGroupBy> CreateInstance(int32_t groupByBuckets, const std::vector<std::pair<std::string, DataType>>& groupByColumns)
	{
		std::vector<DataType> keyDataTypes;

		for (auto& groupByColumn : groupByColumns)
		{
			keyDataTypes.push_back(groupByColumn.second);
		}

		return std::make_unique<GPUGroupBy<OP, O, std::vector<void*>, V>>(Configuration::GetInstance().GetGroupByBuckets(), keyDataTypes);
	}

	static void ProcessBlock(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const PointerAllocation& valueColumn, GpuSqlDispatcher& dispatcher)
	{
		std::vector<void*> keyPtrs;
		std::vector<GPUMemory::GPUString*> stringKeyPtrs;
		int32_t minKeySize = std::numeric_limits<int32_t>::max();

		for (auto& groupByColumn : groupByColumns)
		{
			if (groupByColumn.second == DataType::COLUMN_STRING)
			{
				auto stringColumn = dispatcher.findStringColumn(groupByColumn.first);
				GPUMemory::GPUString* stringColPtr;
				GPUMemory::alloc<GPUMemory::GPUString>(&stringColPtr, 1);

				GPUMemory::GPUString stringCol = std::get<0>(stringColumn);
				GPUMemory::copyHostToDevice<GPUMemory::GPUString>(stringColPtr, &stringCol, 1);
				keyPtrs.push_back(reinterpret_cast<void*>(stringColPtr));
				stringKeyPtrs.push_back(stringColPtr);

				minKeySize = std::min(std::get<1>(stringColumn), minKeySize);
			}
			else
			{
				PointerAllocation column = dispatcher.allocatedPointers.at(groupByColumn.first);
				keyPtrs.push_back(reinterpret_cast<void*>(column.gpuPtr));
				minKeySize = std::min(column.elementCount, minKeySize);
			}
		}

		int32_t dataSize = std::min(minKeySize, valueColumn.elementCount);

		reinterpret_cast<GPUGroupBy<OP, O, std::vector<void*>, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->GroupBy(keyPtrs, reinterpret_cast<V*>(valueColumn.gpuPtr), dataSize);
		
		for (auto& stringPtr : stringKeyPtrs)
		{
			GPUMemory::free(stringPtr);
		}
	}

	static void GetResults(const std::vector<std::pair<std::string, DataType>>& groupByColumns, const std::string& reg, GpuSqlDispatcher& dispatcher)
	{
		int32_t outSize;
		std::vector<void*> outKeys;
		O* outValues = nullptr;
		reinterpret_cast<GPUGroupBy<OP, O, std::vector<void*>, V>*>(dispatcher.groupByTables[dispatcher.dispatcherThreadId].get())->GetResults(&outKeys, &outValues, &outSize, dispatcher.groupByTables);

		for (int32_t i = 0; i < groupByColumns.size(); i++)
		{
			switch (groupByColumns[i].second)
			{
			case DataType::COLUMN_INT:
				dispatcher.InsertRegister(groupByColumns[i].first + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(reinterpret_cast<int32_t*>(outKeys[i])), outSize, true, 0 });
				break;
			case DataType::COLUMN_LONG:
				dispatcher.InsertRegister(groupByColumns[i].first + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(reinterpret_cast<int64_t*>(outKeys[i])), outSize, true, 0 });
				break;
			case DataType::COLUMN_FLOAT:
				dispatcher.InsertRegister(groupByColumns[i].first + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(reinterpret_cast<float*>(outKeys[i])), outSize, true, 0 });
				break;
			case DataType::COLUMN_DOUBLE:
				dispatcher.InsertRegister(groupByColumns[i].first + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(reinterpret_cast<double*>(outKeys[i])), outSize, true, 0 });
				break;
			case DataType::COLUMN_STRING:
				dispatcher.fillStringRegister(*(reinterpret_cast<GPUMemory::GPUString*>(outKeys[i])), groupByColumns[i].first + KEYS_SUFFIX, outSize, true);
				delete reinterpret_cast<GPUMemory::GPUString*>(outKeys[i]); // delete just pointer to struct
				break;
			case DataType::COLUMN_INT8_T:
				dispatcher.InsertRegister(groupByColumns[i].first + KEYS_SUFFIX, PointerAllocation{ reinterpret_cast<uintptr_t>(reinterpret_cast<int8_t*>(outKeys[i])), outSize, true, 0 });
				break;
			default:
				throw std::runtime_error("GROUP BY operation does not support data type " + std::to_string(groupByColumns[i].second));
				break;
			}
		}
		dispatcher.InsertRegister(reg, PointerAllocation{ reinterpret_cast<uintptr_t>(outValues), outSize, true, 0 });
	}
};

/// Implementation of generic aggregation operation based on functor OP
/// Used when GROUP BY Clause is present
/// Loads data on demand
/// If WHERE clause is present filtering is done before agreggation
/// For each block it updates group by hash table
/// To handle multi-gpu functionality - each dipatcher instance signals when it processes its last block
/// The dispatcher instance handling the overall last block waits for all other dispatcher instances to finish their last blocks
/// and saves the result of group by
/// <returns name="statusCode">Finish status code of the operation</returns>
template<typename OP, typename O, typename K, typename V>
int32_t GpuSqlDispatcher::aggregationGroupBy()
{
	auto colTableName = arguments.read<std::string>();
	auto reg = arguments.read<std::string>();

	int32_t loadFlag = loadCol<V>(colTableName);
	if (loadFlag)
	{
		return loadFlag;
	}

	std::cout << "AggGroupBy: " << colTableName << " " << reg << ", thread: " << dispatcherThreadId << std::endl;


	PointerAllocation& column = allocatedPointers.at(colTableName);
	int32_t reconstructOutSize;

	// Reconstruct column only if it is not group by column (if it is group by column it was already reconstructed in GroupByCol)
	if (std::find_if(groupByColumns.begin(), groupByColumns.end(), StringDataTypeComp(colTableName)) == groupByColumns.end())
	{
		V* reconstructOutReg;
		GPUReconstruct::reconstructColKeep<V>(&reconstructOutReg, &reconstructOutSize, reinterpret_cast<V*>(column.gpuPtr), reinterpret_cast<int8_t*>(filter_), column.elementCount);

		if (column.shouldBeFreed)
		{
			GPUMemory::free(reinterpret_cast<void*>(column.gpuPtr));
		}
		else
		{
			column.shouldBeFreed = true;
		}
		column.gpuPtr = reinterpret_cast<uintptr_t>(reconstructOutReg);
		column.elementCount = reconstructOutSize;
	}

	//TODO void param
	if (groupByTables[dispatcherThreadId] == nullptr)
	{
		groupByTables[dispatcherThreadId] = GpuSqlDispatcher::GroupByHelper<OP, O, K, V>::CreateInstance(Configuration::GetInstance().GetGroupByBuckets(), groupByColumns);
	}

	if (aggregatedRegisters.find(reg) == aggregatedRegisters.end())
	{
		std::cout << "Processed block in AggGroupBy." << std::endl;
		GpuSqlDispatcher::GroupByHelper<OP, O, K, V>::ProcessBlock(groupByColumns, column, *this);

		// If last block was processed, reconstruct group by table
		if (isLastBlockOfDevice)
		{
			if (isOverallLastBlock)
			{
				// Wait until all threads finished work
				std::unique_lock<std::mutex> lock(GpuSqlDispatcher::groupByMutex_);
				GpuSqlDispatcher::groupByCV_.wait(lock, [] { return GpuSqlDispatcher::IsGroupByDone(); });

				std::cout << "Reconstructing group by in thread: " << dispatcherThreadId << std::endl;
				
				GpuSqlDispatcher::GroupByHelper<OP, O, K, V>::GetResults(groupByColumns, reg, *this);
			}
			else
			{
				std::cout << "Group by all blocks done in thread: " << dispatcherThreadId << std::endl;
				// Increment counter and notify threads
				std::unique_lock<std::mutex> lock(GpuSqlDispatcher::groupByMutex_);
				GpuSqlDispatcher::IncGroupByDoneCounter();
				GpuSqlDispatcher::groupByCV_.notify_all();
			}
		}
		aggregatedRegisters.insert(reg);
	}
	
	freeColumnIfRegister<V>(colTableName);
	return 0;
}

/// This executes first (dor each block) when GROUP BY clause is used
/// It loads the group by column (if it is firt encountered reference to the column)
/// and filters it according to WHERE clause
/// <returns name="statusCode">Finish status code of the operation</returns>
template<typename T>
int32_t GpuSqlDispatcher::groupByCol()
{
	std::string columnName = arguments.read<std::string>();

	int32_t loadFlag = loadCol<T>(columnName);
	if (loadFlag)
	{
		return loadFlag;
	}

	std::cout << "GroupBy: " << columnName << std::endl;

	PointerAllocation& column = allocatedPointers.at(columnName);

	int32_t reconstructOutSize;
	T* reconstructOutReg;
	GPUReconstruct::reconstructColKeep<T>(&reconstructOutReg, &reconstructOutSize, reinterpret_cast<T*>(column.gpuPtr), reinterpret_cast<int8_t*>(filter_), column.elementCount);

	if (column.shouldBeFreed)
	{
		GPUMemory::free(reinterpret_cast<void*>(column.gpuPtr));
	}
	else
	{
		column.shouldBeFreed = true;
	}
	column.gpuPtr = reinterpret_cast<uintptr_t>(reconstructOutReg);
	column.elementCount = reconstructOutSize;

	if (std::find_if(groupByColumns.begin(), groupByColumns.end(), StringDataTypeComp(columnName)) == groupByColumns.end())
	{
		groupByColumns.push_back({ columnName,  ::GetColumnType<T>() });
	}
	usingGroupBy = true;
	return 0;
}

template<typename T>
int32_t GpuSqlDispatcher::groupByConst()
{
	return 0;
}
