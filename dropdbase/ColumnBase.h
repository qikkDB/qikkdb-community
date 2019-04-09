#pragma once
#include <string>
#include <typeinfo>
#include <vector>

#include "BlockBase.h"
#include "ComplexPolygonFactory.h"
#include "IColumn.h"
#include "Table.h"
#include "Types/ComplexPolygon.pb.h"
#include "Types/Point.pb.h"

namespace std
{
template <>
struct hash<ColmnarDB::Types::Point>
{
    size_t operator()(const ColmnarDB::Types::Point& x) const
    {
        static_assert(sizeof(size_t) == 8, "size_t is not 8 bytes");
        float latitude = x.geopoint().latitude();
        float longitude = x.geopoint().longitude();
        int32_t* iLatitude = reinterpret_cast<int32_t*>(&latitude);
        int32_t* iLongitude = reinterpret_cast<int32_t*>(&longitude);
        return static_cast<size_t>(*iLatitude) | (static_cast<size_t>(*iLongitude) << 32);
    }
};

template <>
struct hash<ColmnarDB::Types::ComplexPolygon>
{
    size_t operator()(const ColmnarDB::Types::ComplexPolygon& x) const
    {
        std::string wkt = ComplexPolygonFactory::WktFromPolygon(x);
        return std::hash<std::string>{}(wkt);
    }
};


template <>
struct equal_to<ColmnarDB::Types::Point>
{
    bool operator()(const ColmnarDB::Types::Point& lhs, const ColmnarDB::Types::Point& rhs) const
    {
        if (std::abs(lhs.geopoint().latitude() - rhs.geopoint().latitude()) >= 0.0001f ||
            std::abs(lhs.geopoint().longitude() - rhs.geopoint().longitude()) >= 0.0001f)
        {
            return false;
        }
        return true;
    }
};

template <>
struct equal_to<ColmnarDB::Types::ComplexPolygon>
{
    bool operator()(const ColmnarDB::Types::ComplexPolygon& lhs, const ColmnarDB::Types::ComplexPolygon& rhs) const
    {
        if (lhs.polygons_size() != rhs.polygons_size())
        {
            return false;
        }

        int32_t polySize = lhs.polygons_size();
        for (int32_t i = 0; i < polySize; i++)
        {
            if (lhs.polygons(i).geopoints_size() != rhs.polygons(i).geopoints_size())
            {
                return false;
            }
            int32_t pointSize = lhs.polygons(i).geopoints_size();
            for (int32_t j = 0; j < pointSize; j++)
            {

                if (std::abs(lhs.polygons(i).geopoints(j).latitude() -
                             rhs.polygons(i).geopoints(j).latitude()) >= 0.0001f ||
                    std::abs(lhs.polygons(i).geopoints(j).longitude() -
                             rhs.polygons(i).geopoints(j).longitude()) >= 0.0001f)
                {
                    return false;
                }
            }
        }
        return true;
    }
};
} // namespace std

template <class T>
class ColumnBase : public IColumn
{
private:
    std::string name_;
    int blockSize_;
    std::vector<std::unique_ptr<BlockBase<T>>> blocks_;

    std::vector<T> NullArray(int length);
    void setColumnStatistics();

    T min_;
    T max_;
    float avg_;
    T sum_;

public:
    ColumnBase(const std::string& name, int blockSize)
    : name_(name), blockSize_(blockSize), blocks_()
    {
    }

    inline int GetBlockSize() const
    {
        return blockSize_;
    };

    virtual const std::string& GetName() const override
    {
        return name_;
    }

    T GetMax()
    {
        return max_;
    }

    T GetMin()
    {
        return min_;
    }

    float GetAvg()
    {
        return avg_;
    }

    T GetSum()
    {
        return sum_;
    }

    /// <summary>
    /// Blocks getter
    /// </summary>
    /// <returns>List of blocks in current column</returns>
    const std::vector<std::unique_ptr<BlockBase<T>>>& GetBlocksList() const
    {
        return blocks_;
    };

    /// <summary>
    /// Add new block in column
    /// </summary>
    /// <returns>Last block of column</returns>
    BlockBase<T>& AddBlock()
    {
        blocks_.push_back(std::make_unique<BlockBase<T>>(*this));
        return *(dynamic_cast<BlockBase<T>*>(blocks_.back().get()));
    }

    /// <summary>
    /// Add new block with proper data into column
    /// </summary>
    /// <param name="data">Data to be inserted</param>
    /// <returns>Last block of column</returns>
    BlockBase<T>& AddBlock(const std::vector<T>& data)
    {
        blocks_.push_back(std::make_unique<BlockBase<T>>(data, *this));
        return *(dynamic_cast<BlockBase<T>*>(blocks_.back().get()));
    }

    std::tuple<int, int, int>
    FindIndexAndRange(int indexBlock, int indexInBlock, int range, const T& columnData)
    {
        int remainingRange = range;
        int blockRange;
        int newRange = 0;
        int newIndexInBlock;
        int newIndexBlock;
        int startIndexInCurrentBlock = indexInBlock;
        bool reachEnd = true;
        bool found = false;

		if (blocks_.size() == 0)
        {
            BlockBase<T>& block = AddBlock();
            newIndexBlock = 0;
            std::tie(newIndexInBlock, newRange, reachEnd) =
                block.FindIndexAndRange(indexInBlock, range, columnData);
		}

		else if (blocks_.size() == 1)
        {
            BlockBase<T>& block = *(blocks_[0].get());
            newIndexBlock = 0;
            std::tie(newIndexInBlock, newRange, reachEnd) =
                block.FindIndexAndRange(indexInBlock, range, columnData);
        }

        else
        {
			for (int i = indexBlock; i < blocks_.size() && reachEnd && remainingRange > 0; i++)
			{
			    BlockBase<T>& block = *(blocks_[i].get());

			    if (columnData >= blocks_[i]->GetMin() &&
			        (remainingRange < block.GetSize() - startIndexInCurrentBlock ||
			         (columnData <= blocks_[i]->GetMax() && (i == blocks_.size() - 1 || columnData <= blocks_[i + 1]->GetMin()))))
			    {
			        int tempIndexInBlock;
			        std::tie(tempIndexInBlock, blockRange, reachEnd) =
			            block.FindIndexAndRange(startIndexInCurrentBlock, remainingRange, columnData);
			        if (!found)
			        {
			            newIndexInBlock = tempIndexInBlock;
			            newIndexBlock = i;
			            found = true;
			        }
			        newRange += blockRange;
			    }
			    remainingRange -= block.GetSize() - startIndexInCurrentBlock;
			    startIndexInCurrentBlock = 0;
			}
			
        }
        return std::make_tuple(newIndexBlock, newIndexInBlock, newRange);
    }

    void InsertDataOnSpecificPosition(int indexBlock, int indexInBlock, const T& columnData)
    {
        if (blocks_.size() == 0)
        {
            BlockBase<T>& block = AddBlock();
        }
        BlockBase<T>& block = *(blocks_[indexBlock].get());
        block.InsertDataOnSpecificPosition(indexInBlock, columnData);

        if (block.IsFull())
        {
            BlockSplit(blocks_[indexBlock]);
        }

        setColumnStatistics();
    }

    void BlockSplit(std::unique_ptr<BlockBase<T>>& blockPtr)
    {
        BlockBase<T>& block = *(blockPtr.get());
        std::vector<T> data1;
        std::vector<T> data2;
        const T* data = block.GetData();

        for (int i = 0; i < block.GetSize(); i++)
        {
            if (i < block.GetSize() / 2)
            {
                data1.push_back(data[i]);
            }
            else
            {
                data2.push_back(data[i]);
            }
        }

        std::unique_ptr<BlockBase<T>> block1 = std::make_unique<BlockBase<T>>(data1, *this);
        std::unique_ptr<BlockBase<T>> block2 = std::make_unique<BlockBase<T>>(data2, *this);

        auto blockIndex = std::find(blocks_.begin(), blocks_.end(), blockPtr);
        blocks_.erase(blockIndex);

        blocks_.insert(blockIndex, std::move(block2));
        blocks_.insert(blockIndex, std::move(block1));
    }


    /// <summary>
    /// Insert data into column considering empty space of last block and maximum size of blocks
    /// </summary>
    /// <param name="columnData">Data to be inserted</param>
    void InsertData(const std::vector<T>& columnData)
    {
        int startIdx = 0;
        if (blocks_.size() > 0 && !blocks_.back()->IsFull())
        {
            auto& lastBlock = blocks_.back();
            if (columnData.size() <= lastBlock->EmptyBlockSpace())
            {
                lastBlock->InsertData(columnData);
                return;
            }
            int emptySpace = lastBlock->EmptyBlockSpace();
            lastBlock->InsertData(std::vector<T>(columnData.cbegin(), columnData.cbegin() + emptySpace));
            startIdx += emptySpace;
        }

        while (startIdx < columnData.size())
        {
            int toCopy = columnData.size() - startIdx < blockSize_ ? columnData.size() - startIdx : blockSize_;
            AddBlock(std::vector<T>(columnData.cbegin() + startIdx, columnData.cbegin() + startIdx + toCopy));
            startIdx += toCopy;
        }
        setColumnStatistics();
    }

    /// <summary>
    /// Get all unique values for this column
    /// </summary>
    /// <returns>Array of unique values</returns>
    std::vector<T> GetUniqueBuckets() const
    {
        std::unordered_set<T> dataSet;
        auto& floatBlocks = GetBlocksList();
        for (const auto& block : floatBlocks)
        {
            for (size_t i = 0; i < block->GetSize(); i++)
            {
                dataSet.insert(block->GetData()[i]);
            }
        }
        return std::vector<T>(dataSet.cbegin(), dataSet.cend());
    }

    /// <summary>
    /// Insert null data into column
    /// </summary>
    /// <param name="length">Length of inserted data</param>
    void InsertNullData(int length)
    {
        InsertData(NullArray(length));
    }

    /// <summary>
    /// Returns type of ColumnBase
    /// </summary>
    /// <returns>Type of current column</returns>
    virtual DataType GetColumnType() const override
    {
        typedef typename std::conditional<
            std::is_same<T, int>::value, std::integral_constant<DataType, COLUMN_INT>,
            typename std::conditional<
                std::is_same<T, int64_t>::value, std::integral_constant<DataType, COLUMN_LONG>,
                typename std::conditional<
                    std::is_same<T, float>::value, std::integral_constant<DataType, COLUMN_FLOAT>,
                    typename std::conditional<
                        std::is_same<T, double>::value, std::integral_constant<DataType, COLUMN_DOUBLE>,
                        typename std::conditional<
                            std::is_same<T, ColmnarDB::Types::Point>::value, std::integral_constant<DataType, COLUMN_POINT>,
                            typename std::conditional<
                                std::is_same<T, ColmnarDB::Types::ComplexPolygon>::value, std::integral_constant<DataType, COLUMN_POLYGON>,
                                typename std::conditional<std::is_same<T, std::string>::value, std::integral_constant<DataType, COLUMN_STRING>,
                                                          typename std::conditional<std::is_same<T, bool>::value, std::integral_constant<DataType, COLUMN_INT8_T>,
                                                                                    typename std::conditional<std::is_same<T, int8_t>::value, std::integral_constant<DataType, COLUMN_INT8_T>, std::integral_constant<DataType, CONST_ERROR>>::type>::
                                                              type>::type>::type>::type>::type>::type>::type>::type retConst;
        return retConst::value;
    };

    virtual int32_t GetBlockCount() const override
    {
        return blocks_.size();
    }
};
