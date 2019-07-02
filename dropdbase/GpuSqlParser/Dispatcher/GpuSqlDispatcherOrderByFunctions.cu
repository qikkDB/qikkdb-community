#include "GpuSqlDispatcherOrderByFunctions.h"

std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> GpuSqlDispatcher::orderByFunctions = { &GpuSqlDispatcher::orderByConst<int32_t>, &GpuSqlDispatcher::orderByConst<int64_t>, &GpuSqlDispatcher::orderByConst<float>, &GpuSqlDispatcher::orderByConst<double>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerConst<ColmnarDB::Types::Point>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerConst<ColmnarDB::Types::ComplexPolygon>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerConst<std::string>, &GpuSqlDispatcher::orderByConst<int8_t>, &GpuSqlDispatcher::orderByCol<int32_t>, &GpuSqlDispatcher::orderByCol<int64_t>, &GpuSqlDispatcher::orderByCol<float>, &GpuSqlDispatcher::orderByCol<double>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerCol<ColmnarDB::Types::Point>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerCol<ColmnarDB::Types::ComplexPolygon>, &GpuSqlDispatcher::invalidOperandTypesErrorHandlerCol<std::string>, &GpuSqlDispatcher::orderByCol<int8_t> };
GpuSqlDispatcher::DispatchFunction GpuSqlDispatcher::freeOrderByTableFunction = &GpuSqlDispatcher::freeOrderByTable;

int32_t GpuSqlDispatcher::freeOrderByTable()
{
	orderByTable.release();
	return 0;
}