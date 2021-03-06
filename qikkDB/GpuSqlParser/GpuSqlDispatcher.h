//
// Created by Martin Staňo on 2019-01-15.
//

#pragma once

#include <functional>
#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <regex>
#include <string>
#include <mutex>
#include <unordered_map>
#include <map>
#include <condition_variable>
#include "../messages/QueryResponseMessage.pb.h"
#include "MemoryStream.h"
#include "../DataType.h"
#include "GroupByType.h"
#include "../QueryEngine/OrderByType.h"
#include "../IVariantArray.h"
#include "../QueryEngine/GPUCore/IGroupBy.h"
#include "../NativeGeoPoint.h"
#include "ParserExceptions.h"
#include "CpuSqlDispatcher.h"
#include "../ConstraintViolationError.h"
#include "../ComplexPolygonFactory.h"
#include "../PointFactory.h"
#include "../QueryEngine/GPUCore/IOrderBy.h"

#ifndef NDEBUG
void AssertDeviceMatchesCurrentThread(int dispatcherThreadId_);
#endif

class Database;
struct InsertIntoStruct;

struct OrderByBlocks
{
    std::unordered_map<std::string, std::vector<std::unique_ptr<IVariantArray>>> ReconstructedOrderByOrderColumnBlocks;
    std::unordered_map<std::string, std::vector<std::unique_ptr<nullmask_t[]>>> ReconstructedOrderByOrderColumnNullBlocks;

    std::unordered_map<std::string, std::vector<std::unique_ptr<IVariantArray>>> ReconstructedOrderByRetColumnBlocks;
    std::unordered_map<std::string, std::vector<std::unique_ptr<nullmask_t[]>>> ReconstructedOrderByRetColumnNullBlocks;
};

class GPUOrderBy;

struct StringDataTypeComp
{
    explicit StringDataTypeComp(const std::string& s) : str(s)
    {
    }
    inline bool operator()(const std::pair<std::string, DataType>& p) const
    {
        return p.first == str;
    }

private:
    const std::string& str;
};


class GpuSqlDispatcher
{
private:
    struct PointerAllocation
    {
        std::uintptr_t GpuPtr = 0;
        int32_t ElementCount = 0;
        bool ShouldBeFreed = false;
        std::uintptr_t GpuNullMaskPtr = 0;
    };

    template <typename T>
    using AllocatedDataType =
        typename std::conditional<std::is_same<typename std::remove_pointer<T>::type, QikkDB::Types::Point>::value,
                                  typename std::conditional<std::is_pointer<T>::value, NativeGeoPoint*, NativeGeoPoint>::type,
                                  T>::type;


    template <typename T>
    using CompositeDataType = typename std::conditional<
        std::is_same<typename std::remove_pointer<T>::type, std::string>::value,
        GPUMemory::GPUString,
        typename std::conditional<std::is_same<typename std::remove_pointer<T>::type, QikkDB::Types::ComplexPolygon>::value, GPUMemory::GPUPolygon, void>::type>::type;


    template <typename T>
    struct CompositeDataTypeAllocation
    {
        CompositeDataType<T> GpuPtr{};
        int32_t ElementCount = 0;
        std::uintptr_t GpuNullMaskPtr = 0;
    };

    enum class InstructionStatus
    {
        CONTINUE = 0,
        OUT_OF_BLOCKS,
        FINISH,
        LOAD_SKIPPED,
        EXCEPTION
    };

    typedef InstructionStatus (GpuSqlDispatcher::*DispatchFunction)();

    template <typename T>
    static constexpr bool isCompositeDataType =
        std::is_same<typename std::remove_pointer<T>::type, std::string>::value ||
        std::is_same<typename std::remove_pointer<T>::type, QikkDB::Types::ComplexPolygon>::value;

    template <typename T>
    struct InstructionArgument
    {
        typename std::conditional<isCompositeDataType<T>, CompositeDataType<typename std::remove_pointer<T>::type>, AllocatedDataType<T>>::type Data;
        typename std::conditional<isCompositeDataType<T>, CompositeDataTypeAllocation<typename std::remove_pointer<T>::type>, PointerAllocation>::type DataAllocation;
        InstructionStatus LoadStatus;
        std::string Name;
    };

    template <typename T>
    struct InstructionResult
    {
        typename std::conditional<isCompositeDataType<T>,
                                  CompositeDataType<typename std::remove_pointer<T>::type>,
                                  typename std::conditional<std::is_pointer<T>::value, AllocatedDataType<T>, AllocatedDataType<T>*>::type>::type Data;
        nullmask_t* NullMaskPtr;
    };

    std::vector<DispatchFunction> dispatcherFunctions_;
    MemoryStream arguments_;
    int32_t blockIndex_;
    int64_t usedRegisterMemory_;
    const int64_t maxRegisterMemory_;
    int32_t dispatcherThreadId_;
    int32_t instructionPointer_;
    int32_t constPointCounter_;
    int32_t constPolygonCounter_;
    int32_t jmpInstructionPosition_;
    int32_t constStringCounter_;
    int64_t loadSize_;
    int64_t loadOffset_;
    const std::shared_ptr<Database>& database_;
    std::string loadedTableName_;
    std::unordered_map<std::string, PointerAllocation> allocatedPointers_;
    std::unordered_map<std::string, std::vector<std::vector<int32_t>>>* joinIndices_;

    QikkDB::NetworkClient::Message::QueryResponseMessage responseMessage_;
    std::uintptr_t filter_;
    bool insideAggregation_;
    bool insideGroupBy_;
    bool insideWhere_;
    bool usingGroupBy_;
    bool usingOrderBy_;
    bool usingJoin_;
    bool isLastBlockOfDevice_;
    bool isOverallLastBlock_;
    bool noLoad_;
    bool aborted_;
    bool groupByHashTableFull_;
    int32_t hashTableMultiplier_;
    int64_t loadNecessary_;
    std::vector<std::pair<std::string, DataType>> groupByColumns_;
    std::unordered_set<std::string> aggregatedRegisters_;
    std::unordered_set<std::string> registerLockList_;
    bool IsRegisterAllocated(const std::string& reg);
    std::pair<std::string, std::string> SplitColumnName(const std::string& colName);
    bool isValidCast(DataType fromType, DataType toType);
    std::vector<std::unique_ptr<IGroupBy>>& groupByTables_;
    CpuSqlDispatcher cpuDispatcher_;

    std::unique_ptr<InsertIntoStruct> insertIntoData_;
    std::unordered_map<std::string, std::vector<nullmask_t>> insertIntoNullMasks_;
    std::unique_ptr<IOrderBy> orderByTable_;
    std::vector<OrderByBlocks>& orderByBlocks_;

    std::unordered_map<std::string, std::unique_ptr<IVariantArray>> reconstructedOrderByColumnsMerged_;
    std::unordered_map<std::string, std::unique_ptr<nullmask_t[]>> reconstructedOrderByColumnsNullMerged_;

    std::unordered_map<int32_t, std::pair<std::string, OrderBy::Order>> orderByColumns_;
    std::vector<std::vector<int32_t>> orderByIndices_;


    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> greaterFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> lessFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> greaterEqualFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> lessEqualFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> equalFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> notEqualFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> logicalAndFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> logicalOrFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> mulFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> divFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> addFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> subFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> modFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> bitwiseOrFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> bitwiseAndFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> bitwiseXorFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> bitwiseLeftShiftFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> bitwiseRightShiftFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> logarithmFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> arctangent2Functions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> roundDecimalFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> concatFunctions;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> powerFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> rootFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> geoLongitudeToTileXFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> geoLatitudeToTileYFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> geoTileXToLongitudeFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> geoTileYToLatitudeFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> pointFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> containsFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> intersectFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> unionFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> leftFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> rightFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToIntFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToLongFunctions_;
    // static std::array<GpuSqlDispatcher::DispatchFunction,
    //		DataType::DATA_TYPE_SIZE> castToDateFunctions;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToFloatFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToDoubleFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToStringFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToPointFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToPolygonFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> castToInt8TFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> logicalNotFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> dateToStringFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> yearFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> monthFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> dayFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> hourFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> minuteFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> secondFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> weekdayFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> dayOfWeekFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> minusFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> absoluteFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> sineFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> cosineFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> tangentFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> cotangentFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> arcsineFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> arccosineFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> arctangentFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> logarithm10Functions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> logarithmNaturalFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> exponentialFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> squareRootFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> squareFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> signFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> roundFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> ceilFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> floorFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> ltrimFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> rtrimFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> lowerFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> upperFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> reverseFunctions_;
    static std::array<GpuSqlDispatcher::DispatchFunction, DataType::DATA_TYPE_SIZE> lenFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> minAggregationFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> maxAggregationFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> sumAggregationFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> countAggregationFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> avgAggregationFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> minGroupByFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> maxGroupByFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> sumGroupByFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> countGroupByFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE * DataType::DATA_TYPE_SIZE> avgGroupByFunctions_;

    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> minGroupByMultiKeyFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> maxGroupByMultiKeyFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> sumGroupByMultiKeyFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> countGroupByMultiKeyFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> avgGroupByMultiKeyFunctions_;

    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> orderByFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> orderByReconstructFunctions_;

    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> retFunctions_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> groupByFunctions_;

    static DispatchFunction isNullFunction_;
    static DispatchFunction isNotNullFunction_;

    static DispatchFunction aggregationBeginFunction_;
    static DispatchFunction aggregationDoneFunction_;
    static DispatchFunction groupByBeginFunction_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE + 1> groupByDoneFunctions_;
    static DispatchFunction freeOrderByTableFunction_;
    static DispatchFunction orderByReconstructRetAllBlocksFunction_;
    static DispatchFunction getLoadSizeFunction_;
    static DispatchFunction filFunction_;
    static DispatchFunction whereEvaluationFunction_;
    static DispatchFunction lockRegisterFunction_;
    static DispatchFunction jmpFunction_;
    static DispatchFunction doneFunction_;
    static DispatchFunction showDatabasesFunction_;
    static DispatchFunction showTablesFunction_;
    static DispatchFunction showColumnsFunction_;
    static DispatchFunction showConstraintsFunction_;
    static DispatchFunction showQueryColumnTypesFunction_;
    static DispatchFunction createDatabaseFunction_;
    static DispatchFunction dropDatabaseFunction_;
    static DispatchFunction createTableFunction_;
    static DispatchFunction dropTableFunction_;
    static DispatchFunction alterTableFunction_;
    static DispatchFunction alterDatabaseFunction_;
    static DispatchFunction createIndexFunction_;
    static std::array<DispatchFunction, DataType::DATA_TYPE_SIZE> insertIntoFunctions_;
    static DispatchFunction insertIntoDoneFunction_;

    static int32_t groupByDoneCounter_;
    static int32_t orderByDoneCounter_;
    static int32_t deviceCountLimit_;
    static int64_t processedDataSize_;

    static std::atomic_bool thrownException_;

    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<int8_t[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);
    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<int32_t[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);

    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<int64_t[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);

    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<float[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);

    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<double[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);

    void InsertIntoPayload(QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                           std::unique_ptr<std::string[]>& data,
                           const int32_t dataSize,
                           const PayloadType payloadType);

    int32_t GetBinaryDispatchTableIndex(DataType left, DataType right);
    int32_t GetUnaryDispatchTableIndex(DataType type);
    void ClearCachedBlocks(const std::string& tableName, const std::string& columnName, const int32_t fromBlockIdx = 0);

    /// If GROUP BY Hash table is filled, restart of the thread
    /// blocks processing is needed - reset flags and start
    /// from thread's first block again
    void ResetBlocksProcessing();

    /// Increase the hash table size and restart the thread
    void HandleHashTableFull();

public:
    static const std::string KEYS_SUFFIX;
    static const std::string NULL_SUFFIX;
    static const std::string RECONSTRUCTED_SUFFIX;

    static std::mutex groupByMutex_;
    static std::mutex orderByMutex_;
    static std::mutex loadSizeMutex_;

    static std::condition_variable groupByCV_;
    static std::condition_variable orderByCV_;

    static void IncGroupByDoneCounter()
    {
        groupByDoneCounter_++;
    }

    static void IncOrderByDoneCounter()
    {
        orderByDoneCounter_++;
    }

    static bool IsGroupByDone()
    {
        return (groupByDoneCounter_ == deviceCountLimit_) || GpuSqlDispatcher::thrownException_;
    }

    static bool IsOrderByDone()
    {
        return (orderByDoneCounter_ == deviceCountLimit_) || GpuSqlDispatcher::thrownException_;
    }

    static void ResetGroupByCounters()
    {
        groupByDoneCounter_ = 0;
        deviceCountLimit_ = 0;
    }

    static void ResetOrderByCounters()
    {
        orderByDoneCounter_ = 0;
        deviceCountLimit_ = 0;
    }

    static void ResetErrorState()
    {
        thrownException_ = false;
    }

    template <typename T>
    static std::pair<bool, T> AggregateOnCPU(std::string& operation, T number1, T number2)
    {
        if (operation == "MIN")
        {
            return std::make_pair(true, number1 < number2 ? number1 : number2);
        }
        else if (operation == "MAX")
        {
            return std::make_pair(true, number1 > number2 ? number1 : number2);
        }
        else if (operation == "SUM" || operation == "AVG" || operation == "COUNT")
        {
            return std::make_pair(true, number1 + number2);
        }
        else // Other operation (e.g. datetime)
        {
            return std::make_pair(false, T{0});
        }
    }

    static void MergePayload(const std::string& key,
                             const std::string& trimmedRealName,
                             QikkDB::NetworkClient::Message::QueryResponseMessage* responseMessage,
                             QikkDB::NetworkClient::Message::QueryResponsePayload& payload);
    static void MergePayloadBitmask(const std::string& key,
                                    QikkDB::NetworkClient::Message::QueryResponseMessage* responseMessage,
                                    std::vector<nullmask_t> nullMask,
                                    int64_t payloadSize);

    template <typename T>
    static void ShiftNullMaskLeft(T& mask, int64_t shift)
    {
        constexpr size_t bits = (sizeof(nullmask_t) * 8) - 1;
        while (shift-- > 0)
        {
            nullmask_t carryBit = 0;
            for (int32_t i = mask.size() - 1; i >= 0; i--)
            {
                nullmask_t newCarryBit = mask.at(i) & static_cast<nullmask_t>(1U);
                mask.at(i) >>= static_cast<nullmask_t>(1U);
                nullmask_t firstZero = ~(static_cast<nullmask_t>(1U) << bits);
                mask.at(i) &= firstZero;
                mask.at(i) |= (carryBit << bits);
                carryBit = newCarryBit;
            }
        }
    }

    GpuSqlDispatcher(const std::shared_ptr<Database>& database,
                     std::vector<std::unique_ptr<IGroupBy>>& groupByTables,
                     std::vector<OrderByBlocks>& orderByBlocks,
                     int dispatcherThreadId);
    ~GpuSqlDispatcher();

    GpuSqlDispatcher(const GpuSqlDispatcher& dispatcher2) = delete;

    GpuSqlDispatcher& operator=(const GpuSqlDispatcher&) = delete;

    void CopyExecutionDataTo(GpuSqlDispatcher& other, CpuSqlDispatcher& sourceCpuDispatcher);

    void SetJoinIndices(std::unordered_map<std::string, std::vector<std::vector<int32_t>>>* joinIdx);

    void Execute(std::unique_ptr<google::protobuf::Message>& result, std::exception_ptr& exception);

    void Abort();

    const QikkDB::NetworkClient::Message::QueryResponseMessage& GetQueryResponseMessage();

    void AddGreaterFunction(DataType left, DataType right);

    void AddLessFunction(DataType left, DataType right);

    void AddGreaterEqualFunction(DataType left, DataType right);

    void AddLessEqualFunction(DataType left, DataType right);

    void AddEqualFunction(DataType left, DataType right);

    void AddNotEqualFunction(DataType left, DataType right);

    void AddLogicalAndFunction(DataType left, DataType right);

    void AddLogicalOrFunction(DataType left, DataType right);

    void AddMulFunction(DataType left, DataType right);

    void AddDivFunction(DataType left, DataType right);

    void AddAddFunction(DataType left, DataType right);

    void AddSubFunction(DataType left, DataType right);

    void AddModFunction(DataType left, DataType right);

    void AddBitwiseOrFunction(DataType left, DataType right);

    void AddBitwiseAndFunction(DataType left, DataType right);

    void AddBitwiseXorFunction(DataType left, DataType right);

    void AddBitwiseLeftShiftFunction(DataType left, DataType right);

    void AddBitwiseRightShiftFunction(DataType left, DataType right);

    void AddPointFunction(DataType left, DataType right);

    void AddContainsFunction(DataType left, DataType right);

    void AddIntersectFunction(DataType left, DataType right);

    void AddUnionFunction(DataType left, DataType right);

    void AddCastToIntFunction(DataType operand);

    void AddCastToLongFunction(DataType operand);

    void AddCastToDateFunction(DataType operand);

    void AddCastToFloatFunction(DataType operand);

    void AddCastToDoubleFunction(DataType operand);

    void AddCastToStringFunction(DataType operand);

    void AddCastToPointFunction(DataType operand);

    void AddCastToPolygonFunction(DataType operand);

    void AddCastToInt8TFunction(DataType operand);

    void AddLogicalNotFunction(DataType type);

    void AddIsNullFunction();

    void AddIsNotNullFunction();

    void AddMinusFunction(DataType type);

    void AddDateToStringFunction(DataType type);

    void AddYearFunction(DataType type);

    void AddMonthFunction(DataType type);

    void AddDayFunction(DataType type);

    void AddHourFunction(DataType type);

    void AddMinuteFunction(DataType type);

    void AddSecondFunction(DataType type);

    void AddWeekdayFunction(DataType type);

    void AddDayOfWeekFunction(DataType type);

    void AddAbsoluteFunction(DataType type);

    void AddSineFunction(DataType type);

    void AddCosineFunction(DataType type);

    void AddTangentFunction(DataType type);

    void AddCotangentFunction(DataType type);

    void AddArcsineFunction(DataType type);

    void AddArccosineFunction(DataType type);

    void AddArctangentFunction(DataType type);

    void AddLogarithm10Function(DataType type);

    void AddLogarithmFunction(DataType number, DataType base);

    void AddArctangent2Function(DataType y, DataType x);

    void AddRoundDecimalFunction(DataType y, DataType x);

    void AddConcatFunction(DataType left, DataType right);

    void AddLeftFunction(DataType left, DataType right);

    void AddRightFunction(DataType left, DataType right);

    void AddLogarithmNaturalFunction(DataType type);

    void AddExponentialFunction(DataType type);

    void AddPowerFunction(DataType base, DataType exponent);

    void AddSquareRootFunction(DataType type);

    void AddSquareFunction(DataType type);

    void AddSignFunction(DataType type);

    void AddRoundFunction(DataType type);

    void AddFloorFunction(DataType type);

    void AddCeilFunction(DataType type);

    void AddLtrimFunction(DataType type);

    void AddRtrimFunction(DataType type);

    void AddLowerFunction(DataType type);

    void AddUpperFunction(DataType type);

    void AddReverseFunction(DataType type);

    void AddLenFunction(DataType type);

    void AddRootFunction(DataType base, DataType exponent);

    void AddGeoLongitudeToTileXFunction(DataType longitude, DataType zoom);

    void AddGeoLatitudeToTileYFunction(DataType latitude, DataType zoom);

    void AddGeoTileXToLongitudeFunction(DataType tileX, DataType zoom);

    void AddGeoTileYToLatitudeFunction(DataType tileY, DataType zoom);

    void AddMinFunction(DataType key, DataType value, GroupByType groupByType);

    void AddMaxFunction(DataType key, DataType value, GroupByType groupByType);

    void AddSumFunction(DataType key, DataType value, GroupByType groupByType);

    void AddCountFunction(DataType key, DataType value, GroupByType groupByType);

    void AddAvgFunction(DataType key, DataType value, GroupByType groupByType);

    void AddRetFunction(DataType type);

    void AddOrderByFunction(DataType type);

    void AddOrderByReconstructFunction(DataType type);

    void AddFreeOrderByTableFunction();

    void AddOrderByReconstructRetAllBlocksFunction();

    void AddLockRegisterFunction();

    void AddFilFunction();

    void AddWhereEvaluationFunction();

    void AddJmpInstruction();

    void AddDoneFunction();

    void AddShowDatabasesFunction();

    void AddShowTablesFunction();

    void AddShowColumnsFunction();

    void AddShowConstraintsFunction();

    void AddShowQueryColumnTypesFunction();

    void AddCreateDatabaseFunction();

    void AddDropDatabaseFunction();

    void AddCreateTableFunction();

    void AddDropTableFunction();

    void AddAlterTableFunction();

    void AddAlterDatabaseFunction();

    void AddCreateIndexFunction();

    void AddInsertIntoFunction(DataType type);

    void AddInsertIntoDoneFunction();

    void AddGroupByFunction(DataType type);

    void AddGroupByBeginFunction();

    void AddGroupByDoneFunction(DataType key, GroupByType groupByType);

    void AddAggregationBeginFunction();

    void AddAggregationDoneFunction();

    void AddBetweenFunction(DataType op1, DataType op2, DataType op3);

    void SetLoadedTableName(const std::string& tableName);

    void AddGetLoadSizeFunction();

    static std::unordered_map<std::string, int32_t> linkTable;

    template <typename T>
    T* AllocateRegister(const std::string& reg, int32_t size, nullmask_t** nullPointerMask = nullptr)
    {
        T* gpuRegister;
        GPUMemory::alloc<T>(&gpuRegister, size);
        if (nullPointerMask)
        {
            int32_t bitMaskSize = NullValues::GetNullBitMaskSize(size);
            GPUMemory::alloc<nullmask_t>(nullPointerMask, bitMaskSize);
            InsertRegister(reg + NULL_SUFFIX, PointerAllocation{reinterpret_cast<std::uintptr_t>(*nullPointerMask),
                                                                bitMaskSize, true, 0});
            InsertRegister(reg, PointerAllocation{reinterpret_cast<std::uintptr_t>(gpuRegister), size, true,
                                                  reinterpret_cast<std::uintptr_t>(*nullPointerMask)});
        }
        else
        {
            InsertRegister(reg, PointerAllocation{reinterpret_cast<std::uintptr_t>(gpuRegister), size, true, 0});
        }

        usedRegisterMemory_ += size * sizeof(T);
        return gpuRegister;
    }

    template <typename T>
    void FillCompositeDataTypeRegister(CompositeDataType<T> column,
                                       const std::string& reg,
                                       int32_t size,
                                       bool useCache = false,
                                       nullmask_t* nullMaskPtr = nullptr);

    /// Check if registerName is contained in allocatedPointers and if so, throw; if not, insert register
    void InsertRegister(const std::string& registerName, PointerAllocation registerValues);

    template <typename T>
    void AddCachedRegister(const std::string& reg, T* ptr, int32_t size, nullmask_t* nullMaskPtr = nullptr)
    {
        InsertRegister(reg, PointerAllocation{reinterpret_cast<std::uintptr_t>(ptr), size, false,
                                              reinterpret_cast<std::uintptr_t>(nullMaskPtr)});
    }

    template <typename T, class Enable = void>
    class DispatcherInstructionHelper
    {
    public:
        static InstructionArgument<T> LoadInstructionArgument(GpuSqlDispatcher& dispatcher)
        {
            PointerAllocation column = {0, 0, false, 0};

            if constexpr (std::is_pointer<T>::value)
            {
                auto colName = dispatcher.arguments_.Read<std::string>();
                GpuSqlDispatcher::InstructionStatus loadFlag =
                    dispatcher.LoadCol<typename std::remove_pointer<T>::type>(colName);

                if (loadFlag != InstructionStatus::CONTINUE)
                {
                    return {reinterpret_cast<AllocatedDataType<T>>(column.GpuPtr), column, loadFlag, colName};
                }

                if (std::find_if(dispatcher.groupByColumns_.begin(), dispatcher.groupByColumns_.end(),
                                 StringDataTypeComp(colName)) != dispatcher.groupByColumns_.end() &&
                    !dispatcher.insideAggregation_ && !dispatcher.insideWhere_)
                {
                    if (dispatcher.isOverallLastBlock_)
                    {
                        column = dispatcher.allocatedPointers_.at(colName + KEYS_SUFFIX);
                    }
                }
                else if (dispatcher.isOverallLastBlock_ || !dispatcher.usingGroupBy_ ||
                         dispatcher.insideGroupBy_ || dispatcher.insideAggregation_ || dispatcher.insideWhere_)
                {
                    column = dispatcher.allocatedPointers_.at(colName);
                }

                return {reinterpret_cast<AllocatedDataType<T>>(column.GpuPtr), column, loadFlag, colName};
            }
            else
            {
                return {dispatcher.arguments_.Read<AllocatedDataType<T>>(), column,
                        InstructionStatus::CONTINUE, ""};
            }
        }

        static InstructionResult<T> AllocateInstructionResult(GpuSqlDispatcher& dispatcher,
                                                              const std::string& reg,
                                                              int32_t retSize,
                                                              bool allocateNullMask,
                                                              const std::vector<std::string>& instructionOperandColumns)
        {
            AllocatedDataType<T>* result = nullptr;
            nullmask_t* nullMask = nullptr;

            bool areGroupByColumns = false;

            for (auto& operandColumn : instructionOperandColumns)
            {
                areGroupByColumns =
                    areGroupByColumns ||
                    (std::find_if(dispatcher.groupByColumns_.begin(), dispatcher.groupByColumns_.end(),
                                  StringDataTypeComp(operandColumn)) != dispatcher.groupByColumns_.end());
            }

            if (areGroupByColumns && !dispatcher.insideAggregation_ && !dispatcher.insideWhere_)
            {
                if (dispatcher.isOverallLastBlock_)
                {
                    result = allocateNullMask ?
                                 dispatcher.AllocateRegister<AllocatedDataType<T>>(reg + KEYS_SUFFIX,
                                                                                   retSize, &nullMask) :
                                 dispatcher.AllocateRegister<AllocatedDataType<T>>(reg + KEYS_SUFFIX, retSize);

                    dispatcher.groupByColumns_.push_back({reg, ::GetColumnType<T>()});
                }
            }
            else if (dispatcher.isOverallLastBlock_ || !dispatcher.usingGroupBy_ ||
                     dispatcher.insideGroupBy_ || dispatcher.insideAggregation_ || dispatcher.insideWhere_)
            {
                if (!dispatcher.IsRegisterAllocated(reg))
                {
                    result = allocateNullMask ?
                                 dispatcher.AllocateRegister<AllocatedDataType<T>>(reg, retSize, &nullMask) :
                                 dispatcher.AllocateRegister<AllocatedDataType<T>>(reg, retSize);
                }
            }
            return {result, nullMask};
        }

        static void StoreInstructionResult(InstructionResult<T> instructionResult,
                                           GpuSqlDispatcher& dispatcher,
                                           const std::string& reg,
                                           int32_t retSize,
                                           bool allocateNullMask,
                                           const std::vector<std::string>& instructionOperandColumns)
        {
            // No explicit result storage required for primitive types
        }
    };

    template <typename T>
    class DispatcherInstructionHelper<T, typename std::enable_if<isCompositeDataType<T>>::type>
    {
    public:
        static InstructionArgument<T> LoadInstructionArgument(GpuSqlDispatcher& dispatcher)
        {
            CompositeDataTypeAllocation<typename std::remove_pointer<T>::type> column;

            if constexpr (std::is_pointer<T>::value)
            {
                auto colName = dispatcher.arguments_.Read<std::string>();
                GpuSqlDispatcher::InstructionStatus loadFlag =
                    dispatcher.LoadCol<typename std::remove_pointer<T>::type>(colName);

                if (loadFlag != InstructionStatus::CONTINUE)
                {
                    return {column.GpuPtr, column, loadFlag, colName};
                }

                if (std::find_if(dispatcher.groupByColumns_.begin(), dispatcher.groupByColumns_.end(),
                                 StringDataTypeComp(colName)) != dispatcher.groupByColumns_.end() &&
                    !dispatcher.insideAggregation_ && !dispatcher.insideWhere_)
                {
                    if (dispatcher.isOverallLastBlock_)
                    {
                        column =
                            dispatcher.FindCompositeDataTypeAllocation<typename std::remove_pointer<T>::type>(
                                colName + KEYS_SUFFIX);
                    }
                }
                else if (dispatcher.isOverallLastBlock_ || !dispatcher.usingGroupBy_ ||
                         dispatcher.insideGroupBy_ || dispatcher.insideAggregation_ || dispatcher.insideWhere_)
                {
                    column =
                        dispatcher.FindCompositeDataTypeAllocation<typename std::remove_pointer<T>::type>(colName);
                }

                return {column.GpuPtr, column, loadFlag, colName};
            }
            else
            {
                const std::string cnst = dispatcher.arguments_.Read<std::string>();
                const int32_t retSize = dispatcher.GetBlockSize();
                if (retSize == 0)
                {
                    return {column.GpuPtr, column, InstructionStatus::OUT_OF_BLOCKS, ""};
                }

                CompositeDataType<T> gpuComposite = dispatcher.InsertConstCompositeDataType<T>(cnst, retSize);
                column = {gpuComposite, 0, 0};
                return {gpuComposite, column, InstructionStatus::CONTINUE, ""};
            }
        }

        static InstructionResult<T> AllocateInstructionResult(GpuSqlDispatcher& dispatcher,
                                                              const std::string& reg,
                                                              int32_t retSize,
                                                              bool allocateNullMask,
                                                              const std::vector<std::string>& instructionOperandColumns)
        {
            CompositeDataType<typename std::remove_pointer<T>::type> result;
            nullmask_t* nullMask = nullptr;

            bool areGroupByColumns = false;

            for (auto& operandColumn : instructionOperandColumns)
            {
                areGroupByColumns =
                    areGroupByColumns ||
                    (std::find_if(dispatcher.groupByColumns_.begin(), dispatcher.groupByColumns_.end(),
                                  StringDataTypeComp(operandColumn)) != dispatcher.groupByColumns_.end());
            }

            if (areGroupByColumns && !dispatcher.insideAggregation_ && !dispatcher.insideWhere_)
            {
                if (dispatcher.isOverallLastBlock_)
                {
                    if (allocateNullMask)
                    {
                        const int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
                        nullMask = dispatcher.AllocateRegister<nullmask_t>(reg + KEYS_SUFFIX + NULL_SUFFIX,
                                                                           bitMaskSize);
                    }
                    dispatcher.groupByColumns_.push_back({reg, ::GetColumnType<T>()});
                }
            }
            else if (dispatcher.isOverallLastBlock_ || !dispatcher.usingGroupBy_ ||
                     dispatcher.insideGroupBy_ || dispatcher.insideAggregation_ || dispatcher.insideWhere_)
            {
                if (!dispatcher.IsRegisterAllocated(reg))
                {
                    if (allocateNullMask)
                    {
                        const int32_t bitMaskSize = NullValues::GetNullBitMaskSize(retSize);
                        nullMask = dispatcher.AllocateRegister<nullmask_t>(reg + NULL_SUFFIX, bitMaskSize);
                    }
                }
            }
            return {result, nullMask};
        }

        static void StoreInstructionResult(InstructionResult<T> instructionResult,
                                           GpuSqlDispatcher& dispatcher,
                                           const std::string& reg,
                                           int32_t retSize,
                                           bool allocateNullMask,
                                           const std::vector<std::string>& instructionOperandColumns)
        {
            bool areGroupByColumns = false;

            for (auto& operandColumn : instructionOperandColumns)
            {
                areGroupByColumns =
                    areGroupByColumns ||
                    (std::find_if(dispatcher.groupByColumns_.begin(), dispatcher.groupByColumns_.end(),
                                  StringDataTypeComp(operandColumn)) != dispatcher.groupByColumns_.end());
            }

            if (areGroupByColumns && !dispatcher.insideAggregation_ && !dispatcher.insideWhere_)
            {
                if (dispatcher.isOverallLastBlock_)
                {
                    dispatcher.FillCompositeDataTypeRegister<typename std::remove_pointer<T>::type>(
                        instructionResult.Data, reg + KEYS_SUFFIX, retSize, true,
                        allocateNullMask ? instructionResult.NullMaskPtr : nullptr);
                }
            }
            else if (dispatcher.isOverallLastBlock_ || !dispatcher.usingGroupBy_ ||
                     dispatcher.insideGroupBy_ || dispatcher.insideAggregation_ || dispatcher.insideWhere_)
            {
                if (!dispatcher.IsRegisterAllocated(reg))
                {
                    dispatcher.FillCompositeDataTypeRegister<typename std::remove_pointer<T>::type>(
                        instructionResult.Data, reg, retSize, true,
                        allocateNullMask ? instructionResult.NullMaskPtr : nullptr);
                }
            }
        }
    };

    template <typename T>
    InstructionStatus LoadCol(std::string& colName);

    InstructionStatus LoadColNullMask(std::string& colName);

    InstructionStatus LoadTableBlockInfo(const std::string& tableName);

    size_t GetBlockSize(int32_t blockIndex = -1);

    int32_t GetBlockCount();

    template <typename T>
    void FreeColumnIfRegister(const std::string& col)
    {
        if (usedRegisterMemory_ > maxRegisterMemory_ && !col.empty() && col.front() == '$' &&
            registerLockList_.find(col) == registerLockList_.end() &&
            allocatedPointers_.find(col) != allocatedPointers_.end())
        {
            CudaLogBoost::getInstance(CudaLogBoost::info) << "Free: " << col << '\n';

            if (allocatedPointers_.at(col).GpuPtr)
            {
                GPUMemory::free(reinterpret_cast<void*>(allocatedPointers_.at(col).GpuPtr));
                usedRegisterMemory_ -= allocatedPointers_.at(col).ElementCount * sizeof(T);
            }
            allocatedPointers_.erase(col);

            if (allocatedPointers_.find(col + NULL_SUFFIX) != allocatedPointers_.end())
            {
                GPUMemory::free(reinterpret_cast<void*>(allocatedPointers_.at(col + NULL_SUFFIX).GpuPtr));
                usedRegisterMemory_ -=
                    allocatedPointers_.at(col + NULL_SUFFIX).ElementCount * sizeof(nullmask_t);
                allocatedPointers_.erase(col + NULL_SUFFIX);
            }
        }
    }

    void FreeRegisterNullMask(const std::string& col);

    // TODO FreeColumnIfRegister<std::string> laso point and polygon
    void MergePayloadToSelfResponse(const std::string& key,
                                    const std::string& realName,
                                    QikkDB::NetworkClient::Message::QueryResponsePayload& payload,
                                    std::vector<nullmask_t> nullMask = {});

    GPUMemory::GPUPolygon InsertComplexPolygon(const std::string& databaseName,
                                               const std::string& colName,
                                               const std::vector<QikkDB::Types::ComplexPolygon>& polygons,
                                               int32_t size,
                                               bool useCache = false,
                                               nullmask_t* nullMaskPtr = nullptr);
    GPUMemory::GPUString InsertString(const std::string& databaseName,
                                      const std::string& colName,
                                      const std::string* strings,
                                      size_t size,
                                      bool useCache = false,
                                      nullmask_t* nullMaskPtr = nullptr);
    template <typename T>
    CompositeDataTypeAllocation<T> FindCompositeDataTypeAllocation(const std::string& colName);
    void RewriteColumn(PointerAllocation& column, uintptr_t newPtr, int32_t newSize, nullmask_t* newNullMask);
    void RewriteStringColumn(const std::string& colName, GPUMemory::GPUString newStruct, int32_t newSize, nullmask_t* newNullMask);

    template <typename T>
    CompositeDataType<T> InsertConstCompositeDataType(const std::string& str, size_t size = 1);

    template <typename T>
    InstructionStatus OrderByConst();

    template <typename T>
    InstructionStatus OrderByCol();

    template <typename T>
    InstructionStatus OrderByReconstructConst();

    template <typename T>
    InstructionStatus OrderByReconstructCol();

    InstructionStatus OrderByReconstructRetAllBlocks();

    template <typename T>
    InstructionStatus RetConst();

    template <typename T>
    InstructionStatus RetCol();

    InstructionStatus AggregationBegin();

    InstructionStatus AggregationDone();

    InstructionStatus GroupByBegin();

    template <typename T>
    InstructionStatus GroupByDone();

    InstructionStatus FreeOrderByTable();

    InstructionStatus LockRegister();

    InstructionStatus GetLoadSize();

    InstructionStatus Fil();

    InstructionStatus WhereEvaluation();

    InstructionStatus Jmp();

    InstructionStatus Done();

    InstructionStatus ShowDatabases();

    InstructionStatus ShowTables();

    InstructionStatus ShowColumns();

    InstructionStatus ShowConstraints();

    InstructionStatus ShowQueryColumnTypes();

    InstructionStatus CreateDatabase();

    InstructionStatus DropDatabase();

    InstructionStatus CreateTable();

    InstructionStatus DropTable();

    InstructionStatus AlterTable();

    InstructionStatus AlterDatabase();

    InstructionStatus CreateIndex();

    void CleanUpGpuPointers();

    template <typename OP, typename L, typename R>
    InstructionStatus Binary();

    template <typename OP, typename T>
    InstructionStatus Unary();

    template <typename OP, typename R, typename T, typename U>
    InstructionStatus AggregationGroupBy();

    template <typename OP, typename OUT, typename IN>
    InstructionStatus AggregationCol();

    template <typename OP, typename T, typename U>
    InstructionStatus AggregationConst();

    InstructionStatus Between();

    template <typename OP>
    InstructionStatus NullMaskCol();

    template <typename T>
    InstructionStatus GroupByCol();

    template <typename T>
    InstructionStatus GroupByConst();

    template <typename T>
    InstructionStatus InsertInto();

    InstructionStatus InsertIntoDone();

    template <typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerColConst()
    {
        U cnst = arguments_.Read<U>();
        auto colName = arguments_.Read<std::string>();

        throw InvalidOperandsException(colName, std::string("cnst"), std::string("operation"));
    }

    template <typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerConstCol()
    {
        auto colName = arguments_.Read<std::string>();
        T cnst = arguments_.Read<T>();

        throw InvalidOperandsException(colName, std::string("cnst"), std::string("operation"));
    }

    template <typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerColCol()
    {
        auto colNameRight = arguments_.Read<std::string>();
        auto colNameLeft = arguments_.Read<std::string>();

        throw InvalidOperandsException(colNameLeft, colNameRight, std::string("operation"));
    }

    template <typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerConstConst()
    {
        U cnstRight = arguments_.Read<U>();
        T cnstLeft = arguments_.Read<T>();

        throw InvalidOperandsException(std::string("cnst"), std::string("cnst"), std::string("operation"));
    }


    //// FUNCTOR ERROR HANDLERS

    template <typename OP, typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerColConst()
    {
        U cnst = arguments_.Read<U>();
        auto colName = arguments_.Read<std::string>();

        throw InvalidOperandsException(colName, std::string("cnst"), std::string(typeid(OP).name()));
    }


    template <typename OP, typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerConstCol()
    {
        auto colName = arguments_.Read<std::string>();
        T cnst = arguments_.Read<T>();

        throw InvalidOperandsException(colName, std::string("cnst"), std::string(typeid(OP).name()));
    }


    template <typename OP, typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerColCol()
    {
        auto colNameRight = arguments_.Read<std::string>();
        auto colNameLeft = arguments_.Read<std::string>();

        throw InvalidOperandsException(colNameLeft, colNameRight, std::string(typeid(OP).name()));
    }


    template <typename OP, typename T, typename U>
    InstructionStatus InvalidOperandTypesErrorHandlerConstConst()
    {
        U cnstRight = arguments_.Read<U>();
        T cnstLeft = arguments_.Read<T>();

        throw InvalidOperandsException(std::string("cnst"), std::string("cnst"),
                                       std::string(typeid(OP).name()));
    }

    template <typename OP, typename T>
    InstructionStatus InvalidOperandTypesErrorHandlerCol()
    {
        auto colName = arguments_.Read<std::string>();

        throw InvalidOperandsException(colName, std::string(""), std::string(typeid(OP).name()));
    }

    template <typename OP, typename T>
    InstructionStatus InvalidOperandTypesErrorHandlerConst()
    {
        T cnst = arguments_.Read<T>();

        throw InvalidOperandsException(std::string(""), std::string("cnst"), std::string(typeid(OP).name()));
    }

    ////

    template <typename T>
    InstructionStatus InvalidOperandTypesErrorHandlerCol()
    {
        auto colName = arguments_.Read<std::string>();

        throw InvalidOperandsException(colName, std::string(""), std::string("operation"));
    }

    template <typename T>
    InstructionStatus InvalidOperandTypesErrorHandlerConst()
    {
        T cnst = arguments_.Read<T>();

        throw InvalidOperandsException(std::string(""), std::string("cnst"), std::string("operation"));
    }

    template <typename T>
    void AddArgument(T argument)
    {
        arguments_.Insert<T>(argument);
    }

    void ClearArguments();
    void ClearInstructions();

private:
    template <typename OP, typename O, typename K, typename V>
    class GroupByHelper;

    template <typename OP, typename O, typename V>
    class GroupByHelper<OP, O, std::string, V>;
};

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetCol<QikkDB::Types::ComplexPolygon>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetCol<QikkDB::Types::Point>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetCol<std::string>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetConst<QikkDB::Types::ComplexPolygon>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetConst<QikkDB::Types::Point>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::RetConst<std::string>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::GroupByCol<std::string>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::InsertInto<QikkDB::Types::ComplexPolygon>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::InsertInto<QikkDB::Types::Point>();

template <>
GpuSqlDispatcher::InstructionStatus
GpuSqlDispatcher::LoadCol<QikkDB::Types::ComplexPolygon>(std::string& colName);

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LoadCol<QikkDB::Types::Point>(std::string& colName);

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::LoadCol<std::string>(std::string& colName);

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::OrderByReconstructCol<std::string>();

template <>
GpuSqlDispatcher::InstructionStatus GpuSqlDispatcher::OrderByReconstructCol<QikkDB::Types::Point>();

template <>
GpuSqlDispatcher::InstructionStatus
GpuSqlDispatcher::OrderByReconstructCol<QikkDB::Types::ComplexPolygon>();

template <>
GpuSqlDispatcher::CompositeDataTypeAllocation<std::string>
GpuSqlDispatcher::FindCompositeDataTypeAllocation<std::string>(const std::string& colName);

template <>
GpuSqlDispatcher::CompositeDataTypeAllocation<QikkDB::Types::ComplexPolygon>
GpuSqlDispatcher::FindCompositeDataTypeAllocation<QikkDB::Types::ComplexPolygon>(const std::string& colName);

template <>
void GpuSqlDispatcher::FillCompositeDataTypeRegister<std::string>(GpuSqlDispatcher::CompositeDataType<std::string> column,
                                                                  const std::string& reg,
                                                                  int32_t size,
                                                                  bool useCache,
                                                                  nullmask_t* nullMaskPtr);

template <>
void GpuSqlDispatcher::FillCompositeDataTypeRegister<QikkDB::Types::ComplexPolygon>(
    GpuSqlDispatcher::CompositeDataType<QikkDB::Types::ComplexPolygon> column,
    const std::string& reg,
    int32_t size,
    bool useCache,
    nullmask_t* nullMaskPtr);

template <>
GpuSqlDispatcher::CompositeDataType<std::string>
GpuSqlDispatcher::InsertConstCompositeDataType<std::string>(const std::string& str, size_t size);

template <>
GpuSqlDispatcher::CompositeDataType<QikkDB::Types::ComplexPolygon>
GpuSqlDispatcher::InsertConstCompositeDataType<QikkDB::Types::ComplexPolygon>(const std::string& str,
                                                                                 size_t size);
