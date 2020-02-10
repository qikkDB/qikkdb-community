#pragma once
#include "../GpuSqlDispatcher.h"
#include "../../QueryEngine/GPUCore/GPULogic.cuh"
#include "../../QueryEngine/GPUCore/GPUFilter.cuh"
#include "../../QueryEngine/GPUCore/GPUArithmetic.cuh"
#include "../../QueryEngine/GPUCore/GPUNullMask.cuh"
#include "../../QueryEngine/GPUCore/GPUMemory.cuh"
#include "../../QueryEngine/GPUCore/GPUFilterConditions.cuh"
#include "GpuSqlDispatcherVMFunctions.h"
#include <tuple>

/// Implementation of generic filter operation (<, >, =, ...) dispatching based on functor OP
/// Implementation for column constant case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterColConst()
{
    U cnst = arguments_.Read<U>();
    auto colName = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<T>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug) << "Filter: " << colName << " " << reg << '\n';

    PointerAllocation column = allocatedPointers_.at(colName);
    int32_t retSize = column.ElementCount;

    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask;
        if (column.GpuNullMaskPtr)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }

        GPUFilter::Filter<OP, T*, U>(mask, reinterpret_cast<T*>(column.GpuPtr), cnst,
                                     reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), retSize);
    }

    FreeColumnIfRegister<T>(colName);
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic filter operation (<, >, =, ...) dispatching based on functor OP
/// Implementation for constant column case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterConstCol()
{
    auto colName = arguments_.Read<std::string>();
    T cnst = arguments_.Read<T>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<U>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug) << "Filter: " << colName << " " << reg << '\n';

    PointerAllocation column = allocatedPointers_.at(colName);
    int32_t retSize = column.ElementCount;

    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask;
        if (column.GpuNullMaskPtr)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }

        GPUFilter::Filter<OP, T, U*>(mask, cnst, reinterpret_cast<U*>(column.GpuPtr),
                                     reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), retSize);
    }

    FreeColumnIfRegister<U>(colName);
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic filter operation (<, >, =, ...) dispatching based on functor OP
/// Implementation for column column case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterColCol()
{
    auto colNameRight = arguments_.Read<std::string>();
    auto colNameLeft = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<U>(colNameRight);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }
    loadFlag = LoadCol<T>(colNameLeft);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "Filter: " << colNameLeft << " " << colNameRight << " " << reg << '\n';

    PointerAllocation columnRight = allocatedPointers_.at(colNameRight);
    PointerAllocation columnLeft = allocatedPointers_.at(colNameLeft);
    int32_t retSize = std::min(columnLeft.ElementCount, columnRight.ElementCount);

    if (!IsRegisterAllocated(reg))
    {
        if (columnLeft.GpuNullMaskPtr || columnRight.GpuNullMaskPtr)
        {
            int64_t* combinedMask;
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize, &combinedMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            if (columnLeft.GpuNullMaskPtr && columnRight.GpuNullMaskPtr)
            {
                GPUArithmetic::Arithmetic<ArithmeticOperations::bitwiseOr>(
                    combinedMask, reinterpret_cast<int64_t*>(columnLeft.GpuNullMaskPtr),
                    reinterpret_cast<int64_t*>(columnRight.GpuNullMaskPtr), bitMaskSize);
            }
            if (columnLeft.GpuNullMaskPtr)
            {
                GPUMemory::copyDeviceToDevice(combinedMask,
                                              reinterpret_cast<int64_t*>(columnLeft.GpuNullMaskPtr), bitMaskSize);
            }
            else if (columnRight.GpuNullMaskPtr)
            {
                GPUMemory::copyDeviceToDevice(combinedMask,
                                              reinterpret_cast<int64_t*>(columnRight.GpuNullMaskPtr), bitMaskSize);
            }
            GPUFilter::Filter<OP, T*, U*>(mask, reinterpret_cast<T*>(columnLeft.GpuPtr),
                                          reinterpret_cast<U*>(columnRight.GpuPtr), combinedMask, retSize);
        }
        else
        {
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
            GPUFilter::Filter<OP, T*, U*>(mask, reinterpret_cast<T*>(columnLeft.GpuPtr),
                                          reinterpret_cast<U*>(columnRight.GpuPtr), nullptr, retSize);
        }
    }

    FreeColumnIfRegister<U>(colNameRight);
    FreeColumnIfRegister<T>(colNameLeft);
    return InstructionStatus::CONTINUE;
}

/// Implementation of genric filter operation (<, >, =, ...) dispatching based on functor OP
/// Implementation for constant constant case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterConstConst()
{
    U constRight = arguments_.Read<U>();
    T constLeft = arguments_.Read<T>();
    auto reg = arguments_.Read<std::string>();

    int32_t retSize = GetBlockSize();
    if (retSize == 0)
    {
        return InstructionStatus::OUT_OF_BLOCKS;
    }
    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
        GPUFilter::Filter<OP, T, U>(mask, constLeft, constRight, nullptr, retSize);
    }
    return InstructionStatus::CONTINUE;
}

template <typename OP>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterStringColConst()
{
    std::string cnst = arguments_.Read<std::string>();
    auto colName = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<std::string>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "FilterStringColConst: " << colName << " " << cnst << " " << reg << '\n';

    auto column = FindStringColumn(colName);
    int32_t retSize = std::get<1>(column);
    int64_t* nullBitMask = std::get<2>(column);

    if (!IsRegisterAllocated(reg))
    {
        GPUMemory::GPUString constString = InsertConstStringGpu(cnst);
        int64_t* mask;
        if (nullBitMask)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, nullBitMask, bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }
        GPUFilter::colConst<OP>(mask, std::get<0>(column), constString, nullBitMask, retSize);
    }
    return InstructionStatus::CONTINUE;
}

template <typename OP>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterStringConstCol()
{
    auto colName = arguments_.Read<std::string>();
    std::string cnst = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<std::string>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "FilterStringConstCol: " << cnst << " " << colName << " " << reg << '\n';

    std::tuple<GPUMemory::GPUString, int32_t, int64_t*> column = FindStringColumn(colName);
    int32_t retSize = std::get<1>(column);
    int64_t* nullBitMask = std::get<2>(column);
    if (!IsRegisterAllocated(reg))
    {
        GPUMemory::GPUString constString = InsertConstStringGpu(cnst);
        int64_t* mask;
        if (nullBitMask)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, nullBitMask, bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }
        GPUFilter::constCol<OP>(mask, constString, std::get<0>(column), nullBitMask, retSize);
    }
    return InstructionStatus::CONTINUE;
}

template <typename OP>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterStringColCol()
{
    auto colNameRight = arguments_.Read<std::string>();
    auto colNameLeft = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<std::string>(colNameRight);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }
    loadFlag = LoadCol<std::string>(colNameLeft);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "FilterStringColCol: " << colNameLeft << " " << colNameRight << " " << reg << '\n';

    std::tuple<GPUMemory::GPUString, int32_t, int64_t*> columnLeft = FindStringColumn(colNameLeft);
    std::tuple<GPUMemory::GPUString, int32_t, int64_t*> columnRight = FindStringColumn(colNameRight);
    int32_t retSize = std::max(std::get<1>(columnLeft), std::get<1>(columnRight));
    int64_t* leftMask = std::get<2>(columnLeft);
    int64_t* rightMask = std::get<2>(columnRight);
    if (!IsRegisterAllocated(reg))
    {
        if (leftMask || rightMask)
        {
            int64_t* combinedMask;
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize, &combinedMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            if (leftMask && rightMask)
            {
                GPUArithmetic::Arithmetic<ArithmeticOperations::bitwiseOr>(combinedMask, leftMask,
                                                                           rightMask, bitMaskSize);
            }
            if (leftMask)
            {
                GPUMemory::copyDeviceToDevice(combinedMask, leftMask, bitMaskSize);
            }
            else if (rightMask)
            {
                GPUMemory::copyDeviceToDevice(combinedMask, rightMask, bitMaskSize);
            }
            GPUFilter::colCol<OP>(mask, std::get<0>(columnLeft), std::get<0>(columnRight), combinedMask, retSize);
        }

        else
        {
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
            GPUFilter::colCol<OP>(mask, std::get<0>(columnLeft), std::get<0>(columnRight), nullptr, retSize);
        }
    }
    return InstructionStatus::CONTINUE;
}


template <typename OP>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::FilterStringConstConst()
{
    std::string cnstRight = arguments_.Read<std::string>();
    std::string cnstLeft = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "FilterStringConstConst: " << cnstLeft << " " << cnstRight << " " << reg << '\n';
    int32_t retSize = GetBlockSize();
    if (retSize == 0)
    {
        return InstructionStatus::OUT_OF_BLOCKS;
    }
    if (!IsRegisterAllocated(reg))
    {
        GPUMemory::GPUString constStringLeft = InsertConstStringGpu(cnstLeft);
        GPUMemory::GPUString constStringRight = InsertConstStringGpu(cnstRight);

        int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
        GPUFilter::constConst<OP>(mask, constStringLeft, constStringRight, retSize);
    }
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic logical operation (AND, OR) dispatching based on functor OP
/// Implementation for column constant case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalColConst()
{
    U cnst = arguments_.Read<U>();
    auto colName = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<T>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    PointerAllocation column = allocatedPointers_.at(colName);
    int32_t retSize = column.ElementCount;

    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask;
        if (column.GpuNullMaskPtr)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }

        GPULogic::Logic<OP, T*, U>(mask, reinterpret_cast<T*>(column.GpuPtr), cnst,
                                   reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), retSize);
    }

    FreeColumnIfRegister<T>(colName);
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic logical operation (AND, OR) dispatching based on functor OP
/// Implementation for constant column case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalConstCol()
{
    auto colName = arguments_.Read<std::string>();
    T cnst = arguments_.Read<T>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<U>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    PointerAllocation column = allocatedPointers_.at(colName);
    int32_t retSize = column.ElementCount;

    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask;
        if (column.GpuNullMaskPtr)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }

        GPULogic::Logic<OP, T, U*>(mask, cnst, reinterpret_cast<U*>(column.GpuPtr),
                                   reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), retSize);
    }

    FreeColumnIfRegister<U>(colName);
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic logical operation (AND, OR) dispatching based on functor OP
/// Implementation for column column case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalColCol()
{
    auto colNameRight = arguments_.Read<std::string>();
    auto colNameLeft = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<U>(colNameRight);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }
    loadFlag = LoadCol<T>(colNameLeft);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug)
        << "Logical: " << colNameLeft << " " << colNameRight << " " << reg << '\n';

    PointerAllocation columnRight = allocatedPointers_.at(colNameRight);
    PointerAllocation columnLeft = allocatedPointers_.at(colNameLeft);

    int32_t retSize = std::min(columnLeft.ElementCount, columnRight.ElementCount);

    if (!IsRegisterAllocated(reg))
    {
        if (columnLeft.GpuNullMaskPtr || columnRight.GpuNullMaskPtr)
        {
            int64_t* combinedMask;
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize, &combinedMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            if (columnLeft.GpuNullMaskPtr && columnRight.GpuNullMaskPtr)
            {
                GPUArithmetic::Arithmetic<ArithmeticOperations::bitwiseOr>(
                    combinedMask, reinterpret_cast<int64_t*>(columnLeft.GpuNullMaskPtr),
                    reinterpret_cast<int64_t*>(columnRight.GpuNullMaskPtr), bitMaskSize);
            }
            if (columnLeft.GpuNullMaskPtr)
            {
                GPUMemory::copyDeviceToDevice(combinedMask,
                                              reinterpret_cast<int64_t*>(columnLeft.GpuNullMaskPtr), bitMaskSize);
            }
            else if (columnRight.GpuNullMaskPtr)
            {
                GPUMemory::copyDeviceToDevice(combinedMask,
                                              reinterpret_cast<int64_t*>(columnRight.GpuNullMaskPtr), bitMaskSize);
            }
            GPULogic::Logic<OP, T*, U*>(mask, reinterpret_cast<T*>(columnLeft.GpuPtr),
                                        reinterpret_cast<U*>(columnRight.GpuPtr), combinedMask, retSize);
        }
        else
        {
            int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
            GPULogic::Logic<OP, T*, U*>(mask, reinterpret_cast<T*>(columnLeft.GpuPtr),
                                        reinterpret_cast<U*>(columnRight.GpuPtr), nullptr, retSize);
        }
    }

    FreeColumnIfRegister<U>(colNameRight);
    FreeColumnIfRegister<T>(colNameLeft);
    return InstructionStatus::CONTINUE;
}

/// Implementation of generic logical operation (AND, OR) dispatching based on functor OP
/// Implementation for constant constant case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename OP, typename T, typename U>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalConstConst()
{
    U constRight = arguments_.Read<U>();
    T constLeft = arguments_.Read<T>();
    auto reg = arguments_.Read<std::string>();
    int32_t retSize = GetBlockSize();
    if (retSize == 0)
    {
        return InstructionStatus::OUT_OF_BLOCKS;
    }
    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask = AllocateRegister<int64_t>(reg, retSize);
        GPULogic::Logic<OP, T, U>(mask, constLeft, constRight, nullptr, retSize);
    }

    return InstructionStatus::CONTINUE;
}

/// Implementation of NOT operation dispatching
/// Implementation for column case
/// Pops data from argument memory stream, and loads data to GPU on demand
/// <returns name="statusCode">Finish status code of the operation</returns>
template <typename T>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalNotCol()
{
    auto colName = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadCol<T>(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    CudaLogBoost::getInstance(CudaLogBoost::debug) << "NotCol: " << colName << " " << reg << '\n';

    PointerAllocation column = allocatedPointers_.at(colName);
    int32_t retSize = column.ElementCount;

    if (!IsRegisterAllocated(reg))
    {
        int64_t* mask;
        if (column.GpuNullMaskPtr)
        {
            int64_t* nullMask;
            mask = AllocateRegister<int64_t>(reg, retSize, &nullMask);
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
            GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), bitMaskSize);
        }
        else
        {
            mask = AllocateRegister<int64_t>(reg, retSize);
        }
        GPULogic::Not<int64_t, T*>(mask, reinterpret_cast<T*>(column.GpuPtr),
                                  reinterpret_cast<int64_t*>(column.GpuNullMaskPtr), retSize);
    }

    FreeColumnIfRegister<T>(colName);
    return InstructionStatus::CONTINUE;
}

template <typename T>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LogicalNotConst()
{
    return InstructionStatus::CONTINUE;
}


template <typename OP>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::NullMaskCol()
{
    auto colName = arguments_.Read<std::string>();
    auto reg = arguments_.Read<std::string>();

    CudaLogBoost::getInstance(CudaLogBoost::debug) << "NullMaskCol: " << colName << " " << reg << '\n';

    if (colName.front() == '$')
    {
        throw NullMaskOperationInvalidOperandException();
    }

    GpuSqlDispatcher::InstructionStatus loadFlag = LoadColNullMask(colName);
    if (loadFlag != InstructionStatus::CONTINUE)
    {
        return loadFlag;
    }

    PointerAllocation columnMask = allocatedPointers_.at(colName + NULL_SUFFIX);
    size_t nullMaskSize = NullValues::GetNullBitMaskSize(columnMask.ElementCount);

    if (!IsRegisterAllocated(reg))
    {
        int64_t* outFilterMask;

        int64_t* nullMask;
        outFilterMask = AllocateRegister<int64_t>(reg, columnMask.ElementCount, &nullMask);
        GPUMemory::copyDeviceToDevice(nullMask, reinterpret_cast<int64_t*>(columnMask.GpuPtr), nullMaskSize);
        GPUNullMask::Col<OP>(outFilterMask, reinterpret_cast<int64_t*>(columnMask.GpuPtr),
                             nullMaskSize, columnMask.ElementCount);
    }
    return InstructionStatus::CONTINUE;
}
