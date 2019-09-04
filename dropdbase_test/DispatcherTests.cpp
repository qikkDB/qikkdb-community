#include <cmath>
#include <functional>
#include <iostream>
#include <fstream>

#include "gtest/gtest.h"
#include "../dropdbase/DatabaseGenerator.h"
#include "../dropdbase/ColumnBase.h"
#include "../dropdbase/BlockBase.h"
#include "../dropdbase/PointFactory.h"
#include "../dropdbase/ComplexPolygonFactory.h"
#include "../dropdbase/Database.h"
#include "../dropdbase/Table.h"
#include "../dropdbase/QueryEngine/Context.h"
#include "../dropdbase/GpuSqlParser/GpuSqlCustomParser.h"
#include "../dropdbase/messages/QueryResponseMessage.pb.h"
#include "../dropdbase/QueryEngine/OrderByType.h"
#include "../dropdbase/GpuSqlParser/LoadColHelper.h"
#include "DispatcherObjs.h"

/////////////////////
//   ">" operator
/////////////////////

// INT ">"
TEST(DispatcherTests, IntGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024) > 5)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 > 5)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 500 > colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 > j % 1024)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (500 > (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger2 FROM TableA WHERE colInteger2 > colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 2048) > (j % 1024))
                {
                    expectedResult.push_back(j % 2048);
                }
            }
            else
            {
                if ((j % 2048) * -1 > (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger2");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 > 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 > 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG ">"
TEST(DispatcherTests, LongGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 > 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 > 500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 > 500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 > colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500000000 > static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (500000000 > (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong2 FROM TableA WHERE colLong2 > colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 > 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 > 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT ">"
TEST(DispatcherTests, FloatGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 > 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) > 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.5 > colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<float>(j % 1024 + 0.1111) < 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if (static_cast<float>((j % 1024 + 0.1111) * -1) < 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * -1));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat2 FROM TableA WHERE colFloat2 > colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 2048 + 0.1111)) > (static_cast<float>(j % 1024 + 0.1111)))
                {
                    expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat2");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10 > 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 > 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE ">"
TEST(DispatcherTests, DoubleGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 > 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) > 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) > 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.5 > colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) < 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) < 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble2 FROM TableA WHERE colDouble2 > colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 2048 + 0.1111111) > (j % 1024 + 0.1111111))
                {
                    expectedResult.push_back(j % 2048 + 0.1111111);
                }
            }
            else
            {
                if (((j % 2048 + 0.1111111) * (-1)) > ((j % 1024 + 0.1111111) * (-1)))
                {
                    expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble2");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10 > 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 > 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "<" operator
/////////////////////

// INT "<"
TEST(DispatcherTests, IntLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 < 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024) < 5)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 < 5)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 500 < colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 < j % 1024)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (500 < (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 < colInteger2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024) < (j % 2048))
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 < (j % 2048) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 < 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 < 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "<"
TEST(DispatcherTests, LongLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 < 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 < 500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 < 500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 < colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500000000 < static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (500000000 < (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 < colLong2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 < 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 < 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT "<"
TEST(DispatcherTests, FloatLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 < 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<float>(j % 1024 + 0.1111) < 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if (static_cast<float>((j % 1024 + 0.1111) * -1) < 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * -1));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.5 < colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<float>(j % 1024 + 0.1111) > 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if (static_cast<float>((j % 1024 + 0.1111) * -1) > 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * -1));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 < colFloat2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) < (static_cast<float>(j % 2048 + 0.1111)))
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) <
                    (static_cast<float>((j % 2048 + 0.1111) * (-1))))
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 < 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10 < 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE "<"
TEST(DispatcherTests, DoubleLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 < 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) < 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) < 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.5 < colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) > 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) > 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 < colDouble2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) < (j % 2048 + 0.1111111))
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) < ((j % 2048 + 0.1111111) * (-1)))
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 < 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10 < 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   ">=" operator
/////////////////////

// INT ">="
TEST(DispatcherTests, IntEqGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 >= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 >= 5)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 500 >= colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 >= j % 1024)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (500 >= (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger2 FROM TableA WHERE colInteger2 >= colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 2048) >= (j % 1024))
                {
                    expectedResult.push_back(j % 2048);
                }
            }
            else
            {
                if (((j % 2048) * -1) >= (j % 1024) * -1)
                {
                    (j % 2) ? expectedResult.push_back(j % 2048) : expectedResult.push_back((j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger2");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 >= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 >= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG ">="
TEST(DispatcherTests, LongEqGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 >= 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 >= 500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 >= 500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 >= colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500000000 >= static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (500000000 >= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database,
                              "SELECT colLong2 FROM TableA WHERE colLong2 >= colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 >= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 >= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT ">="
TEST(DispatcherTests, FloatEqGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 >= 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) >= 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) >= 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.5 >= colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) <= 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) <= 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat2 FROM TableA WHERE colFloat2 >= colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 2048 + 0.1111)) >= (static_cast<float>(j % 1024 + 0.1111)))
                {
                    expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 2048 + 0.1111) * (-1))) >=
                    (static_cast<float>((j % 1024 + 0.1111) * (-1))))
                {
                    expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat2");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10 >= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 >= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE ">="
TEST(DispatcherTests, DoubleEqGtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 >= 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) >= 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqGtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.5 >= colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) <= 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) <= 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqGtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble2 FROM TableA WHERE colDouble2 >= colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 2048 + 0.1111111) >= (j % 1024 + 0.1111111))
                {
                    expectedResult.push_back(j % 2048 + 0.1111111);
                }
            }
            else
            {
                if (((j % 2048 + 0.1111111) * (-1)) >= ((j % 1024 + 0.1111111) * (-1)))
                {
                    expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble2");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqGtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10 >= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqGtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 >= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "<=" operator
/////////////////////

// INT "<="
TEST(DispatcherTests, IntEqLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 <= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024) <= 5)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 <= 5)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 500 <= colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 <= j % 1024)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (500 <= (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 <= colInteger2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024) <= (j % 2048))
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 <= (j % 2048) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 <= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 <= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "<="
TEST(DispatcherTests, LongEqLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 <= 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 <= 500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 <= 500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }


    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 <= colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500000000 <= static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (500000000 <= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 <= colLong2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 <= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 <= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT "<="
TEST(DispatcherTests, FloatEqLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 <= 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) <= 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) <= 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.5 <= colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) >= 5.5)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) >= 5.5)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 <= colFloat2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<float>(j % 1024 + 0.1111)) <= (static_cast<float>(j % 2048 + 0.1111)))
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) <=
                    (static_cast<float>((j % 2048 + 0.1111) * (-1))))
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 <= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10 <= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE "<="
TEST(DispatcherTests, DoubleEqLtColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 <= 5.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) <= 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) <= 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqLtConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.5 <= colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) >= 5.5)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) >= 5.5)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqLtColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 <= colDouble2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) <= (j % 2048 + 0.1111111))
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) <= ((j % 2048 + 0.1111111) * (-1)))
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqLtConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 <= 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqLtConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10 <= 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "=" operator
/////////////////////

// INT "="
TEST(DispatcherTests, IntEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 = 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if ((j % 1024) == 5)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 = colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (5 == j % 1024)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger2 FROM TableA WHERE colInteger2 = colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 2048) == (j % 1024))
            {
                (j % 2) ? expectedResult.push_back(j % 2048) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger2");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 = 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 = 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "="
TEST(DispatcherTests, LongEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 = 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 = colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong2 FROM TableA WHERE colLong2 = colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) ==
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1) ==
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 = 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 = 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT "="
TEST(DispatcherTests, FloatEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 = 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 = colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat2 FROM TableA WHERE colFloat2 = colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (std::abs((static_cast<float>(j % 2048 + 0.1111)) - (static_cast<float>(j % 1024 + 0.1111))) < 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat2");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 = 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 = 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE "="
/*TEST(DispatcherTests, DoubleEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colDouble1 FROM TableA WHERE colDouble1 = 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((j % 1024 + 0.1111111) - 5.1111111) < 0.00000005)
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
    }

    auto &payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}*/

/*TEST(DispatcherTests, DoubleEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colDouble1 FROM TableA WHERE 5.1111111 = colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((j % 1024 + 0.1111111) - 5.1111111) < 0.00000005)
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
    }

    auto &payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}*/

TEST(DispatcherTests, DoubleEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble2 FROM TableA WHERE colDouble2 = colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (std::abs((j % 2048 + 0.1111111) - (j % 1024 + 0.1111111)) < 0.00000005)
            {
                (j % 2) ? expectedResult.push_back(j % 2048 + 0.1111111) :
                          expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble2");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 = 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 = 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "!=" operator
/////////////////////

// INT "!="
TEST(DispatcherTests, IntNotEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if ((j % 1024) != 5)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntNotEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 500 != colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 != j % 1024)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (500 != (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntNotEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger2 FROM TableA WHERE colInteger2 != colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 2048) != (j % 1024))
                {
                    expectedResult.push_back(j % 2048);
                }
            }
            else
            {
                if ((j % 2048) * -1 != (j % 1024) * -1)
                {
                    expectedResult.push_back((j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger2");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntNotEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntNotEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "!="
TEST(DispatcherTests, LongNotEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 != 50000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 50000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 50000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongNotEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE -500000000 != colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != -500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != -500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongNotEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong2 FROM TableA WHERE colLong2 != colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) !=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 !=
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongNotEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 != 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongNotEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT "!="
TEST(DispatcherTests, FloatNotEqColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 != 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatNotEqConstColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 != colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatNotEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat2 FROM TableA WHERE colFloat2 != colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (std::abs((static_cast<float>(j % 2048 + 0.1111)) - (static_cast<float>(j % 1024 + 0.1111))) > 0.00005)
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat2");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatNotEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 != 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatNotEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE "!="
/*TEST(DispatcherTests, DoubleNotEqColumnConst) //FIXME test je dobry, chyba je v kerneli
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colDouble1 FROM TableA WHERE colDouble1 != 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (std::abs((j % 1024 + 0.1111111) - 5.1111111) > std::numeric_limits<double>::epsilon())
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (std::abs(((j % 1024 + 0.1111111) * (-1)) - 5.1111111) > std::numeric_limits<double>::epsilon())
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto &payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}*/

/*TEST(DispatcherTests, DoubleNotEqConstColumn) //FIXME test je dobry, chyba je v kerneli
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colDouble1 FROM TableA WHERE 5.1111111 != colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (std::abs((j % 1024 + 0.1111111) - 5.1111111) > std::numeric_limits<double>::epsilon())
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                if (std::abs(((j % 1024 + 0.1111111) * (-1)) - 5.1111111) > std::numeric_limits<double>::epsilon())
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto &payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}*/

TEST(DispatcherTests, DoubleNotEqColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble2 FROM TableA WHERE colDouble2 != colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (std::abs((j % 2048 + 0.1111111) - (j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(j % 2048 + 0.1111111) :
                          expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble2");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleNotEqConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 != 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleNotEqConstConstFalse)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5 != 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   AND
/////////////////////

// INT AND
TEST(DispatcherTests, IntAndColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 AND 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if ((j % 1024) != 0)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAndColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 AND colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
            if ((j % 1024) != 0)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAndConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 AND colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 AND colInteger2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 2048 != 0) && (j % 1024 != 0))
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAndConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 AND 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAndConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 AND 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG AND
TEST(DispatcherTests, LongAndColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 AND 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAndColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}
TEST(DispatcherTests, LongAndConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 AND colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAndConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 AND colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 AND colLong2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048 != 0) &&
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 != 0) &&
                    ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0))
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAndConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 AND 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAndConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 AND 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT AND
TEST(DispatcherTests, FloatAndColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 AND 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatAndColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 AND colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatAndConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 AND colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat2 AND colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) &&
                (static_cast<float>(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}
TEST(DispatcherTests, FloatAndConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10.1111 AND 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatAndConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 AND 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE AND
TEST(DispatcherTests, DoubleAndColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 AND 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
            {
                (j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) :
                          expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleAndColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.1111111 AND colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                          expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleAndConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 AND colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble2 AND colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 2048 + 0.1111111 > std::numeric_limits<double>::epsilon()) &&
                (j % 1024 + 0.1111111 > std::numeric_limits<double>::epsilon()))
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                          expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}
TEST(DispatcherTests, DoubleAndConstConstTrue)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10.1111111 AND 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleAndConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.11111111 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 AND 5.11111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 AND 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}


/////////////////////
//   OR
/////////////////////

// INT OR
TEST(DispatcherTests, IntOrColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 OR 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 1024) != 0)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 5 OR colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 OR colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 1024) != 0)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 OR colInteger2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 2048 != 0) || (j % 1024 != 0))
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstConstNonZeroValues)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 OR 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 10 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 OR 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntOrConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE 0 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// LONG OR
TEST(DispatcherTests, LongOrColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 OR 500000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 500000000 OR colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 OR colLong1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 OR colLong2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048 != 0) ||
                    (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0))
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 != 0) ||
                    ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0))
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstConstNonZeroValues)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 10 OR 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 5 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 OR 5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongOrConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE 0 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// FLOAT OR
TEST(DispatcherTests, FloatOrColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 OR 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 OR colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 OR colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat2 OR colFloat1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((static_cast<float>(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) ||
                (static_cast<float>(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
            {
                (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                          expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}
TEST(DispatcherTests, FloatOrConstConstNonZeroValues)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 10.1111 OR 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 5.1111 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 OR 5.1111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) :
                      expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatOrConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE 0 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

// DOUBLE OR
TEST(DispatcherTests, DoubleOrColumnConstNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 OR 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) :
                      expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrColumnConstZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
            {
                (j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) :
                          expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstColumnNonZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.1111111 OR colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstColumnZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 OR colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                          expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrColumnColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble2 OR colDouble1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 2048 + 0.1111111 > std::numeric_limits<double>::epsilon()) ||
                (j % 1024 + 0.1111111 > std::numeric_limits<double>::epsilon()))
            {
                (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                          expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstConstNonZeroValues)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 10.1111111 OR 5.1111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstConstFalseRightZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 5.11111111 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstConstFalseLeftZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 OR 5.11111111;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) :
                      expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleOrConstConstFalseBothZero)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE 0 OR 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   NEGATION
/////////////////////

// INT NEGATION
TEST(DispatcherTests, IntNegation)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE !(colInteger1 > 5);");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 <= 5)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024) * -1 <= 5)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

// LONG NEGATION
TEST(DispatcherTests, LongNegation)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE !(colLong1 > 500000000);");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 <= 500000000)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 <= 500000000)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

// FLOAT NEGATION
TEST(DispatcherTests, FloatNegation)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE !(colFloat1 > 6.5555);");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<float>(j % 1024 + 0.1111) <= 6.5555)
                {
                    expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111));
                }
            }
            else
            {
                if (static_cast<float>((j % 1024 + 0.1111) * -1) <= 6.5555)
                {
                    expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * -1));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

// DOUBLE NEGATION
TEST(DispatcherTests, DoubleNegation)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE !(colDouble1 > 9.66666666);");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111111) <= 9.66666666)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111));
                }
            }
            else
            {
                if (((j % 1024 + 0.1111111) * (-1)) <= 9.66666666)
                {
                    expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

///////////

TEST(DispatcherTests, IntAddColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 + 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                expectedResult.push_back(j % 1024 + 5);
            }
            else
            {
                expectedResult.push_back(((j % 1024) * -1) + 5);
            }
        }
    }

    auto& payloads = result->payloads().at("colInteger1+5");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAddColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 + 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 + 5 > 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((j % 1024 + 5) * -1 > 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntAddColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 + 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) + 5) < 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if ((((j % 1024) * -1) + 5) < 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongAddColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 + 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) + 5) :
                expectedResult.push_back((((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)) * -1) + 5);
        }
    }

    auto& payloads = result->payloads().at("colLong1+5");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAddColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 + 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) + 5) > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)) * -1 + 5) > 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongAddColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 + 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) + 5) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)) * -1 + 5) < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, FloatAddColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 + 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) + 5.1111) :
                      expectedResult.push_back(((j % 1024) + 0.1111) * (-1) + 5);
        }
    }

    auto& payloads = result->payloads().at("colFloat1+5");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatAddColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 + 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) + 5.1111) > 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111);
                }
            }
            else
            {
                if (((((j % 1024) + 0.1111) * (-1)) + 5) > 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatAddColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 + 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) + 5.1111) < 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111);
                }
            }
            else
            {
                if ((((j % 1024) + 0.1111) * (-1) + 5) < 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, DoubleAddColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 + 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) + 5.1111111) :
                      expectedResult.push_back(((j % 1024) + 0.1111111) * (-1) + 5);
        }
    }

    auto& payloads = result->payloads().at("colDouble1+5");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleAddColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 + 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 + 5.1111111 > 500)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleAddColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 + 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                double temp = j % 1024 + 0.1111111;

                if (temp + 5 < 500)
                {
                    expectedResult.push_back(j % 1024 + 0.1111111);
                }
            }
            else
            {
                double temp = ((j % 1024) + 0.1111111) * -1;

                if (temp + 5 < 500)
                {
                    expectedResult.push_back((j % 1024 + 0.1111111) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, IntSubColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 - 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) - 5) :
                      expectedResult.push_back(((j % 1024) * -1) - 5);
        }
    }

    auto& payloads = result->payloads().at("colInteger1-5");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntSubColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 - 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 - 5 > 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (((j % 1024) * -1) - 5 > 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntSubColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 - 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 - 5 < 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (((j % 1024) * -1) - 5 < 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongSubColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 - 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) - 5) :
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 - 5);
        }
    }

    auto& payloads = result->payloads().at("colLong1-5");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongSubColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 - 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) - 5) > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 - 5) > 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongSubColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 - 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) - 5) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 - 5) < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, FloatSubColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 - 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(j % 1024 + 0.1111 - 5) :
                      expectedResult.push_back(((j % 1024 + 0.1111) * -1) - 5);
        }
    }

    auto& payloads = result->payloads().at("colFloat1-5");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_TRUE(std::abs(expectedResult[i] - payloads.floatpayload().floatdata()[i]) < 0.0005);
    }
}

TEST(DispatcherTests, FloatSubColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 - 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111 - 5) > 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111);
                }
            }
            else
            {
                if ((((j % 1024 + 0.1111) * -1) - 5) > 500)
                {
                    expectedResult.push_back((j % 1024 + 0.1111) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatSubColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 - 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (j % 1024 + 0.1111 - 5 < 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111) * -1) - 5 < 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, DoubleSubColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 - 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) + 0.1111111 - 5) :
                      expectedResult.push_back((((j % 1024) + 0.1111111) * (-1)) - 5);
        }
    }

    auto& payloads = result->payloads().at("colDouble1-5");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleSubColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 - 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((((j % 1024) + 0.1111111) - 5) > 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111111);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleSubColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 - 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) + 0.1111111 - 5) < 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111111);
                }
            }
            else
            {
                if ((((j % 1024) + 0.1111111) * (-1) - 5) < 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

// multiply tests:
TEST(DispatcherTests, IntMulColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 * 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) * 5) :
                      expectedResult.push_back(((j % 1024) * 5) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("colInteger1*5");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntMulColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 * 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) * 5) > 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (((j % 1024) * -1) * 5 > 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntMulColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 * 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((j % 1024) * 5) < 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (((j % 1024) * -1) * 5 < 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongMulColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 * 2 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * 2) :
                expectedResult.push_back(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) * 2);
        }
    }

    auto& payloads = result->payloads().at("colLong1*2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongMulColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 * 2 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * 2) > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongMulColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 * 2 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * 2) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if ((((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) * 2) < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, FloatMulColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 * 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111) * 5) :
                      expectedResult.push_back((((j % 1024) + 0.1111) * 5) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("colFloat1*5");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_TRUE(std::abs(expectedResult[i] - payloads.floatpayload().floatdata()[i]) < 0.0005);
    }
}

TEST(DispatcherTests, FloatMulColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 * 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111) * 5 > 500)
                {
                    expectedResult.push_back(j % 1024 + 0.1111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111) * -1) * 5 > 500)
                {
                    expectedResult.push_back((j % 1024 + 0.1111) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatMulColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 * 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((j % 1024 + 0.1111) * 5 < 500)
                {
                    expectedResult.push_back(j % 1024 + 0.1111);
                }
            }
            else
            {
                if (((j % 1024 + 0.1111) * -1) * 5 < 500)
                {
                    expectedResult.push_back((j % 1024 + 0.1111) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, DoubleMulColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 * 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111111) * 5) :
                      expectedResult.push_back((((j % 1024) + 0.1111111) * 5) * ((-1)));
        }
    }

    auto& payloads = result->payloads().at("colDouble1*5");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleMulColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 * 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((((j % 1024) + 0.1111111) * 5) > 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111111);
                }
            }
            else
            {
                if (((((j % 1024) + 0.1111111) * (-1)) * 5) > 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleMulColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 * 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((((j % 1024) + 0.1111111) * 5) < 500)
                {
                    expectedResult.push_back((j % 1024) + 0.1111111);
                }
            }
            else
            {
                if (((((j % 1024) + 0.1111111) * (-1)) * 5) < 500)
                {
                    expectedResult.push_back(((j % 1024) + 0.1111111) * ((-1)));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

// divide tests:
TEST(DispatcherTests, IntDivColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 / 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<int32_t>((j % 1024) / 5)) :
                      expectedResult.push_back(static_cast<int32_t>(((j % 1024) / 5) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("colInteger1/5");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntDivColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntDivColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int32_t>((j % 1024) / 5) < 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (static_cast<int32_t>((j % 1024) * -1) / 5 < 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 / 2 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<int64_t>(
                          (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 2)) :
                      expectedResult.push_back(static_cast<int64_t>(
                          ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 2));
        }
    }

    auto& payloads = result->payloads().at("colLong1/2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 / 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 5)) > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 5) > 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 / 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 5)) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 5) < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, FloatDivColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 / 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                expectedResult.push_back((j % 1024 + 0.1111) / 5);
            }
            else
            {
                expectedResult.push_back(((j % 1024 + 0.1111) * -1) / 5);
            }
        }
    }

    auto& payloads = result->payloads().at("colFloat1/5");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloatDivColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 / 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatDivColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colFloat1 / 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((((j % 1024) + 0.1111) / 5) < 500)
            {
                (j % 2) ? expectedResult.push_back((j % 1024) + 0.1111) :
                          expectedResult.push_back(((j % 1024) + 0.1111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, DoubleDivColumnConst) // FIXME Dispatch je chybny, treba ho opravit, test je dobry
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 / 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111111) / 5) :
                      expectedResult.push_back((((j % 1024) + 0.1111111) * -1) / 5);
        }
    }

    auto& payloads = result->payloads().at("colDouble1/5");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, DoubleDivColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 / 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleDivColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colDouble1 FROM TableA WHERE colDouble1 / 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<double> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((((j % 1024) + 0.1111111) / 5) < 500)
            {
                (j % 2) ? expectedResult.push_back((j % 1024) + 0.1111111) :
                          expectedResult.push_back(((j % 1024) + 0.1111111) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colDouble1");

    ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

    for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
    {
        ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
    }
}

TEST(DispatcherTests, IntDivColumnConstFloat) // FIXME chyba je v CUDA kerneli, ma vracat float
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 / 5.0 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back((j % 1024) / 5.0) :
                      expectedResult.push_back(((j % 1024) * -1) / 5.0);
        }
    }

    auto& payloads = result->payloads().at("colInteger1/5.0");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, IntDivColumnConstGtConstFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5.0 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntDivColumnConstLtConstFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5.0 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if ((j % 1024) / 5.0 < 500)
            {
                (j % 2) ? expectedResult.push_back(j % 1024) :
                          expectedResult.push_back((j % 1024) * ((-1)));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConstFloat) // FIXME test je dobry, kernel treba spravi tak aby to pretypoval na double
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 / 2.0 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ?
                expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 2.0) :
                expectedResult.push_back(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 2.0);
        }
    }

    auto& payloads = result->payloads().at("colLong1/2.0");

    ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConstGtConstFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 / 5.0 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 5.0 > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 5.0 > 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongDivColumnConstLtConstFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 / 5.0 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 5.0) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 5.0 < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

// modulo tests:
// divide tests:
TEST(DispatcherTests, IntModColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 % 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<int32_t>((j % 1024) % 5)) :
                      expectedResult.push_back(static_cast<int32_t>(((j % 1024) % 5) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("colInteger1%5");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, IntModColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 % 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntModColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 % 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int32_t>((j % 1024) % 5) < 500)
                {
                    expectedResult.push_back(j % 1024);
                }
            }
            else
            {
                if (static_cast<int32_t>(((j % 1024) * -1) % 5) < 500)
                {
                    expectedResult.push_back((j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LongModColumnConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 % 2 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            (j % 2) ? expectedResult.push_back(static_cast<int64_t>(
                          (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) % 2)) :
                      expectedResult.push_back(static_cast<int64_t>(
                          (static_cast<int64_t>((2 * pow(10, j % 19)) + j % 1024) % 2) * (-1)));
        }
    }

    auto& payloads = result->payloads().at("colLong1%2");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, LongModColumnConstGtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 % 5 > 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongModColumnConstLtConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colLong1 FROM TableA WHERE colLong1 % 5 < 500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int64_t> expectedResult;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) % 5)) < 500)
                {
                    expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
                }
            }
            else
            {
                if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) % 5) < 500)
                {
                    expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colLong1");

    ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

    for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
    {
        ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, DateTimeNow)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT YEAR(NOW()), MONTH(NOW()), DAY(NOW()), HOUR(NOW()), "
                              "MINUTE(NOW()) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsYear;
    std::vector<int32_t> expectedResultsMonth;
    std::vector<int32_t> expectedResultsDay;
    std::vector<int32_t> expectedResultsHour;
    std::vector<int32_t> expectedResultsMinute;
    std::vector<int32_t> expectedResultsSecond;

    std::time_t epochTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* localTime = gmtime(&epochTime);

    expectedResultsYear.push_back(localTime->tm_year + 1900);
    expectedResultsMonth.push_back(localTime->tm_mon + 1);
    expectedResultsDay.push_back(localTime->tm_mday);
    expectedResultsHour.push_back(localTime->tm_hour);
    expectedResultsMinute.push_back(localTime->tm_min);

    auto& payloadsYear = result->payloads().at("YEAR(NOW())");
    auto& payloadsMonth = result->payloads().at("MONTH(NOW())");
    auto& payloadsDay = result->payloads().at("DAY(NOW())");
    auto& payloadsHour = result->payloads().at("HOUR(NOW())");
    auto& payloadsMinute = result->payloads().at("MINUTE(NOW())");

    for (int i = 0; i < expectedResultsYear.size(); i++)
    {
        ASSERT_EQ(expectedResultsYear[i], payloadsYear.intpayload().intdata()[i]);
    }

    for (int i = 0; i < expectedResultsYear.size(); i++)
    {
        ASSERT_EQ(expectedResultsMonth[i], payloadsMonth.intpayload().intdata()[i]);
    }

    for (int i = 0; i < expectedResultsYear.size(); i++)
    {
        ASSERT_EQ(expectedResultsDay[i], payloadsDay.intpayload().intdata()[i]);
    }

    for (int i = 0; i < expectedResultsYear.size(); i++)
    {
        ASSERT_EQ(expectedResultsHour[i], payloadsHour.intpayload().intdata()[i]);
    }

    for (int i = 0; i < expectedResultsYear.size(); i++)
    {
        ASSERT_EQ(expectedResultsMinute[i], payloadsMinute.intpayload().intdata()[i]);
    }
}


// DateTime tests
TEST(DispatcherTests, DateTimeCol)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT YEAR(colLong3), MONTH(colLong3), DAY(colLong3), "
                              "HOUR(colLong3), MINUTE(colLong3), SECOND(colLong3) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsYear;
    std::vector<int32_t> expectedResultsMonth;
    std::vector<int32_t> expectedResultsDay;
    std::vector<int32_t> expectedResultsHour;
    std::vector<int32_t> expectedResultsMinute;
    std::vector<int32_t> expectedResultsSecond;

    for (int i = 0; i < 2; i++)
    {
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsYear.push_back(static_cast<int32_t>(k % 1000) + 2000);
            expectedResultsMonth.push_back(static_cast<int32_t>((k % 12) + 1));
            expectedResultsDay.push_back(static_cast<int32_t>(((k % 28) + 1)));
            expectedResultsHour.push_back(static_cast<int32_t>((k % 24)));
            expectedResultsMinute.push_back(static_cast<int32_t>(((k + 1) % 60)));
            expectedResultsSecond.push_back(static_cast<int32_t>((k + 2) % 60));
        }
    }

    auto& payloadsYear = result->payloads().at("YEAR(colLong3)");
    auto& payloadsMonth = result->payloads().at("MONTH(colLong3)");
    auto& payloadsDay = result->payloads().at("DAY(colLong3)");
    auto& payloadsHour = result->payloads().at("HOUR(colLong3)");
    auto& payloadsMinute = result->payloads().at("MINUTE(colLong3)");
    auto& payloadsSecond = result->payloads().at("SECOND(colLong3)");

    ASSERT_EQ(payloadsYear.intpayload().intdata_size(), expectedResultsYear.size());
    ASSERT_EQ(payloadsMonth.intpayload().intdata_size(), expectedResultsMonth.size());
    ASSERT_EQ(payloadsDay.intpayload().intdata_size(), expectedResultsDay.size());
    ASSERT_EQ(payloadsHour.intpayload().intdata_size(), expectedResultsHour.size());
    ASSERT_EQ(payloadsMinute.intpayload().intdata_size(), expectedResultsMinute.size());
    ASSERT_EQ(payloadsSecond.intpayload().intdata_size(), expectedResultsSecond.size());

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsYear[i], payloadsYear.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsMonth[i], payloadsMonth.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsDay[i], payloadsDay.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsHour[i], payloadsHour.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsMinute[i], payloadsMinute.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsYear.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsSecond[i], payloadsSecond.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, RetPolygons)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPolygon1 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPolygons;

    auto column =
        dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(DispatcherObjs::GetInstance()
                                                                        .database->GetTables()
                                                                        .at("TableA")
                                                                        .GetColumns()
                                                                        .at("colPolygon1")
                                                                        .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsPolygons.push_back(ComplexPolygonFactory::WktFromPolygon(block->GetData()[k], true));
        }
    }

    auto& payloads = result->payloads().at("TableA.colPolygon1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPolygons.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPolygons[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, RetPolygonsWhere)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPolygon1 FROM TableA WHERE colInteger1 < 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPolygons;

    auto column =
        dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(DispatcherObjs::GetInstance()
                                                                        .database->GetTables()
                                                                        .at("TableA")
                                                                        .GetColumns()
                                                                        .at("colPolygon1")
                                                                        .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((k % 1024) * (k % 2 ? 1 : -1) < 20)
            {
                expectedResultsPolygons.push_back(
                    ComplexPolygonFactory::WktFromPolygon(block->GetData()[k], true));
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colPolygon1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPolygons.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPolygons[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, RetPoints)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPoint1 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;

    auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance()
                                                                         .database->GetTables()
                                                                         .at("TableA")
                                                                         .GetColumns()
                                                                         .at("colPoint1")
                                                                         .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k], true));
        }
    }

    auto& payloads = result->payloads().at("TableA.colPoint1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, RetPointsWhere)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPoint1 FROM TableA WHERE colInteger1 < 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;

    auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance()
                                                                         .database->GetTables()
                                                                         .at("TableA")
                                                                         .GetColumns()
                                                                         .at("colPoint1")
                                                                         .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (k % 2)
            {
                if ((k % 1024) < 20)
                {
                    expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k], true));
                }
            }
            else
            {
                if (((k % 1024) * -1) < 20)
                {
                    expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k], true));
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colPoint1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, RetString)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colString1 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsStrings;

    auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colString1")
                                                             .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k]);
        }
    }

    auto& payloads = result->payloads().at("TableA.colString1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsStrings.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsStrings[i], payloads.stringpayload().stringdata()[i]) << " at row " << i;
    }
}

TEST(DispatcherTests, RetStringWhere)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colString1 FROM TableA WHERE colInteger1 < 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsStrings;

    auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colString1")
                                                             .get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (k % 2)
            {
                if ((k % 1024) < 20)
                {
                    expectedResultsStrings.push_back(block->GetData()[k]);
                }
            }
            else
            {
                if (((k % 1024) * -1) < 20)
                {
                    expectedResultsStrings.push_back(block->GetData()[k]);
                }
            }
        }
    }

    auto& payloads = result->payloads().at("TableA.colString1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsStrings.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsStrings[i], payloads.stringpayload().stringdata()[i]) << " at row " << i;
    }
}

TEST(DispatcherTests, PointFromColCol)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT POINT(colInteger1, colFloat1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::ostringstream wktStream;
            wktStream << std::fixed;
            wktStream << std::setprecision(4);
            wktStream << "POINT(" << static_cast<float>(blockInt->GetData()[k]) << " "
                      << blockFloat->GetData()[k] << ")";
            expectedResultsPoints.push_back(wktStream.str());
        }
    }

    auto& payloads = result->payloads().at("POINT(colInteger1,colFloat1)");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, PointFromColConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT POINT(colInteger1, 4.5) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::ostringstream wktStream;
            wktStream << std::fixed;
            wktStream << std::setprecision(4);
            wktStream << "POINT(" << static_cast<float>(blockInt->GetData()[k]) << " 4.5000"
                      << ")";
            expectedResultsPoints.push_back(wktStream.str());
        }
    }

    auto& payloads = result->payloads().at("POINT(colInteger1,4.5)");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, PointFromConstCol)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT POINT(7, colFloat1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;

    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::ostringstream wktStream;
            wktStream << std::fixed;
            wktStream << std::setprecision(4);
            wktStream << "POINT("
                      << "7.0000 " << blockFloat->GetData()[k] << ")";
            expectedResultsPoints.push_back(wktStream.str());
        }
    }

    auto& payloads = result->payloads().at("POINT(7,colFloat1)");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
    }
}

// Aggregation tests
TEST(DispatcherTests, AggregationMin)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT MIN(colInteger1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("MIN(colInteger1)");

    // Get the input column
    const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
        reinterpret_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                   .database->GetTables()
                                                   .at("TableA")
                                                   .GetColumns()
                                                   .at("colInteger1")
                                                   .get())
            ->GetBlocksList();

    // Find min on CPU
    int32_t expectedResult = std::numeric_limits<int32_t>::max();
    for (int i = 0; i < TEST_BLOCK_COUNT; i++)
    {
        for (int j = 0; j < TEST_BLOCK_SIZE; j++)
        {
            int32_t value = inputColumn1Blocks[i]->GetData()[j];
            if (value < expectedResult)
            {
                expectedResult = value;
            }
        }
    }

    ASSERT_EQ(payloads.intpayload().intdata_size(), 1);
    ASSERT_EQ(payloads.intpayload().intdata()[0], expectedResult);
}

TEST(DispatcherTests, AggregationMax)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT MAX(colInteger1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("MAX(colInteger1)");

    // Get the input column
    const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
        reinterpret_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                   .database->GetTables()
                                                   .at("TableA")
                                                   .GetColumns()
                                                   .at("colInteger1")
                                                   .get())
            ->GetBlocksList();

    // Find min on CPU
    int32_t expectedResult = std::numeric_limits<int32_t>::min();
    for (int i = 0; i < TEST_BLOCK_COUNT; i++)
    {
        for (int j = 0; j < TEST_BLOCK_SIZE; j++)
        {
            int32_t value = inputColumn1Blocks[i]->GetData()[j];
            if (value > expectedResult)
            {
                expectedResult = value;
            }
        }
    }

    ASSERT_EQ(payloads.intpayload().intdata_size(), 1);
    ASSERT_EQ(payloads.intpayload().intdata()[0], expectedResult);
}

TEST(DispatcherTests, AggregationSum)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT SUM(colInteger1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("SUM(colInteger1)");

    // Get the input column
    const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
        reinterpret_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                   .database->GetTables()
                                                   .at("TableA")
                                                   .GetColumns()
                                                   .at("colInteger1")
                                                   .get())
            ->GetBlocksList();

    // Find min on CPU
    int32_t expectedResult = 0;
    for (int i = 0; i < TEST_BLOCK_COUNT; i++)
    {
        for (int j = 0; j < TEST_BLOCK_SIZE; j++)
        {
            expectedResult += inputColumn1Blocks[i]->GetData()[j];
        }
    }

    ASSERT_EQ(payloads.intpayload().intdata_size(), 1);
    ASSERT_EQ(payloads.intpayload().intdata()[0], expectedResult);
}

TEST(DispatcherTests, AggregationAvg)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT AVG(colInteger1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("AVG(colInteger1)");

    // Get the input column
    const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
        reinterpret_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                   .database->GetTables()
                                                   .at("TableA")
                                                   .GetColumns()
                                                   .at("colInteger1")
                                                   .get())
            ->GetBlocksList();

    // Find min on CPU
    int32_t expectedResult = 0; // TODO float, also in disptacher fields float
    int64_t count = 0;
    for (int i = 0; i < TEST_BLOCK_COUNT; i++)
    {
        for (int j = 0; j < TEST_BLOCK_SIZE; j++)
        {
            expectedResult += inputColumn1Blocks[i]->GetData()[j];
            ++count;
        }
    }
    expectedResult /= count;

    ASSERT_EQ(payloads.intpayload().intdata_size(), 1);
    ASSERT_EQ(payloads.intpayload().intdata()[0], expectedResult);
}

TEST(DispatcherTests, AggregationCount)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT COUNT(colInteger1) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("COUNT(colInteger1)");
    ASSERT_EQ(payloads.int64payload().int64data_size(), 1);
    ASSERT_EQ(payloads.int64payload().int64data()[0], TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
}


TEST(DispatcherTests, Alias)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT (t.colInteger1 - 10) AS col1, t.colFloat1 AS col2 FROM "
                              "TableA as t WHERE t.colInteger1 > 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;
    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k] - 10);
                expectedResultsFloat.push_back(blockFloat->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("col1");
    auto& payloadsFloat = result->payloads().at("col2");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());
    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}


TEST(DispatcherTests, LimitOffset)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10 "
                              "OFFSET 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto limit = 10;
    auto offset = 10;

    auto first = expectedResultsInt.begin() + offset;
    auto last = expectedResultsInt.begin() + offset + limit;
    std::vector<int32_t> trimmedExpectedResultsInt(first, last);

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, Limit)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto limit = 10;

    auto first = expectedResultsInt.begin();
    auto last = expectedResultsInt.begin() + limit;
    std::vector<int32_t> trimmedExpectedResultsInt(first, last);

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, Offset)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 OFFSET 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto offset = 10;

    auto first = expectedResultsInt.begin() + offset;
    auto last = expectedResultsInt.end();
    std::vector<int32_t> trimmedExpectedResultsInt(first, last);

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LargeOffset)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 OFFSET "
                              "10000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());


    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LargeLimit)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT "
                              "10000000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseOrColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 | 20) > 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] | 20) > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseAndColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 & 20) > 10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] & 20) > 10)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseXorColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ 20) > 100;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] ^ 20) > 100)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseLeftShiftColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 << 2) > 100;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] << 2) > 100)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseRightShiftColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 >> 2) > 100;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] >> 2) > 100)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseOrColColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 | colInteger2) > "
                              "500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colInteger2")
                                                             .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockInt2 = columnInt2->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] | blockInt2->GetData()[k]) > 500)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseAndColColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 & colInteger2) > "
                              "10;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colInteger2")
                                                             .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockInt2 = columnInt2->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] & blockInt2->GetData()[k]) > 10)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, BitwiseXorColColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ colInteger2) > "
                              "500;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colInteger2")
                                                             .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockInt2 = columnInt2->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if ((blockInt->GetData()[k] ^ blockInt2->GetData()[k]) > 500)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, NotEqualsAlternativeOperator)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE colInteger1 <> 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] != 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, MinusColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE -colInteger1 = 3;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, AbsColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE ABS(colInteger1) = 3;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (abs(blockInt->GetData()[k]) == 3)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SinColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIN(colInteger1) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (sin(blockInt->GetData()[k]) > 0.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, CosColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE COS(colInteger1) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (cos(blockInt->GetData()[k]) > 0.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, TanColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE TAN(colInteger1) > 2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (tan(blockInt->GetData()[k]) > 2)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SinColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                                .database->GetTables()
                                                                .at("TableA")
                                                                .GetColumns()
                                                                .at("colInteger1")
                                                                .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInteger = columnInteger->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (sin(blockFloat->GetData()[k]) > 0.5)
            {
                expectedResultsInt.push_back(blockInteger->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, CosColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE COS(colFloat1) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                                .database->GetTables()
                                                                .at("TableA")
                                                                .GetColumns()
                                                                .at("colInteger1")
                                                                .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInteger = columnInteger->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (cos(blockFloat->GetData()[k]) > 0.5)
            {
                expectedResultsInt.push_back(blockInteger->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, TanColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE TAN(colFloat1) > 2;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                                .database->GetTables()
                                                                .at("TableA")
                                                                .GetColumns()
                                                                .at("colInteger1")
                                                                .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInteger = columnInteger->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (tan(blockFloat->GetData()[k]) > 2)
            {
                expectedResultsInt.push_back(blockInteger->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SinPiColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1 + PI()) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                                .database->GetTables()
                                                                .at("TableA")
                                                                .GetColumns()
                                                                .at("colInteger1")
                                                                .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInteger = columnInteger->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (sin(blockFloat->GetData()[k] + pi()) > 0.5f)
            {
                expectedResultsInt.push_back(blockInteger->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, ArcSinColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE ASIN(colInteger1 / 1024.0) > "
                              "0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (asin(blockInt->GetData()[k] / 1024.0f) > 0.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, ArcCosColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE ACOS(colInteger1 / 1024.0) > "
                              "0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (acos(blockInt->GetData()[k] / 1024.0f) > 0.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, ArcTanColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE ATAN(colInteger1) > 0.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (atan(blockInt->GetData()[k]) > 0.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, Logarithm10ColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE LOG10(colInteger1) > 1.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (log10f(blockInt->GetData()[k]) > 1.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LogarithmNaturalColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1) > 1.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (logf(blockInt->GetData()[k]) > 1.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, LogarithmColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1, 3.0) > 1.5;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (logf(blockInt->GetData()[k]) / logf(3) > 1.5)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}
TEST(DispatcherTests, ExponentialColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE EXP(colInteger1) > 2000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (exp(blockInt->GetData()[k]) > 2000)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, PowerColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE POW(colInteger1, 2) > 2000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (pow(blockInt->GetData()[k], 2) > 2000)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SqrtColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SQRT(colInteger1) > 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (sqrtf(blockInt->GetData()[k]) > 20)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SquareColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SQUARE(colInteger1) > 2000;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (powf(blockInt->GetData()[k], 2) > 2000)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SignPositiveColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] > 0)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SignNegativeColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = -1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] < 0)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, SignZeroColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] == 0)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, RootColConstInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA WHERE ROOT(colInteger1, 2) > 0;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (powf(blockInt->GetData()[k], 0.5f) > 0)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k]);
            }
        }
    }

    std::cout << "Expected result size: " << expectedResultsInt.size() << std::endl;

    auto& payloadsInt = result->payloads().at("TableA.colInteger1");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}


TEST(DispatcherTests, RoundColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT ROUND(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(round(blockFloat->GetData()[k]));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("ROUND(colFloat1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, FloorColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT FLOOR(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(floor(blockFloat->GetData()[k]));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("FLOOR(colFloat1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, CeilColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT CEIL(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(ceil(blockFloat->GetData()[k]));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("CEIL(colFloat1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, RoundColInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT ROUND(colInteger1) FROM TableA WHERE colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(round(blockInt->GetData()[k]));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("ROUND(colInteger1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, CotColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT COT(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(1.0f / tanf(blockFloat->GetData()[k]));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("COT(colFloat1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, Atan2ColFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT ATAN2(colFloat1, colFloat1 + 1) FROM TableA WHERE "
                              "colInteger1 >= 20;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] >= 20)
            {
                expectedResultsFloat.push_back(atan2f(blockFloat->GetData()[k], blockFloat->GetData()[k] + 1));
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("ATAN2(colFloat1,colFloat1+1)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}


//== STRING FUNCTIONS ==
/// Assert equality of returned string column and expected values
void AssertEqStringCol(ColmnarDB::NetworkClient::Message::QueryResponsePayload payloads,
                       std::vector<std::string> expected)
{
    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expected.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expected[i], payloads.stringpayload().stringdata()[i]) << " at row " << i;
    }
}

/// Run query SELECT <col> <fromWhere>;
ColmnarDB::NetworkClient::Message::QueryResponsePayload RunQuery(std::string col, std::string fromWhere)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT " + col + " " + fromWhere + ";");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    return result->payloads().at(col);
}

/// Run query SELECT function(column) FROM table; and return result payload
ColmnarDB::NetworkClient::Message::QueryResponsePayload
RunFunctionQuery(std::string function, std::string column, std::string table)
{
    std::string retFunCol = function + "(" + column + ")";
    return RunQuery(retFunCol, "FROM " + table);
}

/// Run query SELECT function(column) FROM table; and return result payload
ColmnarDB::NetworkClient::Message::QueryResponsePayload
RunFunctionColConstQuery(std::string function, std::string column, std::string cnst, std::string table)
{
    std::string retFunCol = function + "(" + column + "," + cnst + ")";
    return RunQuery(retFunCol, "FROM " + table);
}

TEST(DispatcherTests, StringLower)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("LOWER", col, table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::string edited;
            for (char c : block->GetData()[k])
            {
                edited += tolower(c);
            }
            expectedResultsStrings.push_back(edited);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLowerConst)
{
    const std::string text = "\"ABCDabcdzZ [{|}]_#90\"";
    auto payloads = RunFunctionQuery("LOWER", text, "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;
    std::string edited;
    for (char c : text.substr(1, text.length() - 2))
    {
        edited += tolower(c);
    }
    expectedResultsStrings.push_back(edited);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringUpper)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("UPPER", col, table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::string edited;
            for (char c : block->GetData()[k])
            {
                edited += toupper(c);
            }
            expectedResultsStrings.push_back(edited);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringUpperConst)
{
    const std::string text = "\"ABCDabcdzZ [{|}]_#90\"";
    auto payloads = RunFunctionQuery("UPPER", text, "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;
    std::string edited;
    for (char c : text.substr(1, text.length() - 2))
    {
        edited += toupper(c);
    }
    expectedResultsStrings.push_back(edited);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringReverse)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("REVERSE", col, table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::string edited(block->GetData()[k]);
            std::reverse(edited.begin(), edited.end());
            expectedResultsStrings.push_back(edited);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringReverseConst)
{
    const std::string text = "\"ABCDabcdzZ [{|}]_#90\"";
    auto payloads = RunFunctionQuery("REVERSE", text, "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;
    std::string edited(text.substr(1, text.length() - 2));
    std::reverse(edited.begin(), edited.end());
    expectedResultsStrings.push_back(edited);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLtrim)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("LTRIM", col, table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::string rowString(block->GetData()[k]);
            size_t index = rowString.find_first_not_of(' ');
            std::string trimmed;
            if (index < rowString.length())
            {
                trimmed = rowString.substr(index, rowString.length() - index);
            }
            expectedResultsStrings.push_back(trimmed);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLtrimConst)
{
    const std::string text = "\"  ABCDabcdzZ [{|}]_#90  \"";
    auto payloads = RunFunctionQuery("LTRIM", text, "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;

    std::string edited(text.substr(1, text.length() - 2));
    size_t index = edited.find_first_not_of(' ');
    std::string trimmed;
    if (index < edited.length())
    {
        trimmed = edited.substr(index, edited.length() - index);
    }
    expectedResultsStrings.push_back(trimmed);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringRtrim)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("RTRIM", col, table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            std::string rowString(block->GetData()[k]);
            size_t index = rowString.find_last_not_of(' ');
            std::string trimmed = rowString.substr(0, index + 1);
            expectedResultsStrings.push_back(trimmed);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringRtrimConst)
{
    const std::string text = "\"  ABCDabcdzZ [{|}]_#90  \"";
    auto payloads = RunFunctionQuery("RTRIM", text, "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;

    std::string edited(text.substr(1, text.length() - 2));
    ;
    size_t index = edited.find_last_not_of(' ');
    std::string trimmed = edited.substr(0, index + 1);
    expectedResultsStrings.push_back(trimmed);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLen)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    auto payloads = RunFunctionQuery("LEN", col, table);

    std::vector<int32_t> expectedResults;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResults.push_back(block->GetData()[k].length());
        }
    }

    ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResults.size());

    for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResults[i], payloads.intpayload().intdata()[i]) << " at row " << i;
    }
}

TEST(DispatcherTests, StringLenConst)
{
    const std::string text = "\"  ABCDabcdzZ [{|}]_#90  \"";
    auto payloads = RunFunctionQuery("LEN", text, "TableA LIMIT 1");

    ASSERT_EQ(payloads.intpayload().intdata_size(), 1);
    ASSERT_EQ(payloads.intpayload().intdata()[0], text.length() - 2); // - 2 because of two quotes ""
}

TEST(DispatcherTests, StringLeftColCol)
{
    const std::string colStrName = "colString1";
    const std::string colIntName = "colInteger1";
    const std::string table = "TableA";
    const int32_t testLen = 2;
    auto payloads = RunFunctionColConstQuery("LEFT", colStrName, "ABS(" + colIntName + ")", table);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());
    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colIntName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k].substr(0, abs(blockInt->GetData()[k])));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLeftColConst)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    const int32_t testLen = 2;
    auto payloads = RunFunctionColConstQuery("LEFT", col, std::to_string(testLen), table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k].substr(0, testLen));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLeftConstCol)
{
    const std::string text = "  ABCDabcdzZ [{|}]_#90  ";
    const std::string colIntName = "colInteger1";
    const std::string table = "TableA";
    auto payloads = RunFunctionColConstQuery("LEFT", "\"" + text + "\"", "ABS(" + colIntName + ")", table);

    std::vector<std::string> expectedResultsStrings;
    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colIntName).get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(text.substr(0, abs(blockInt->GetData()[k])));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringLeftConstConst)
{
    const std::string text = "  ABCDabcdzZ [{|}]_#90  ";
    const int32_t testLen = 2;
    auto payloads = RunFunctionColConstQuery("LEFT", "\"" + text + "\"", std::to_string(testLen), "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;

    expectedResultsStrings.push_back(text.substr(0, testLen));

    AssertEqStringCol(payloads, expectedResultsStrings);
}


TEST(DispatcherTests, StringRightColCol)
{
    const std::string colStrName = "colString1";
    const std::string colIntName = "colInteger1";
    const std::string table = "TableA";
    auto payloads = RunFunctionColConstQuery("RIGHT", colStrName, "ABS(" + colIntName + ")", table);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());
    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colIntName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            int32_t cutLen = abs(blockInt->GetData()[k]);
            size_t origLen = block->GetData()[k].size();
            expectedResultsStrings.push_back(block->GetData()[k].substr(origLen < cutLen ? 0 : origLen - cutLen));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringRightColConst)
{
    const std::string col = "colString1";
    const std::string table = "TableA";
    const int32_t testLen = 3;
    auto payloads = RunFunctionColConstQuery("RIGHT", col, std::to_string(testLen), table);

    std::vector<std::string> expectedResultsStrings;
    auto column = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(col).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            size_t len = block->GetData()[k].size();
            expectedResultsStrings.push_back(block->GetData()[k].substr(len < testLen ? 0 : len - testLen));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringRightConstCol)
{
    const std::string text = "  ABCDabcdzZ [{|}]_#90  ";
    const std::string colIntName = "colInteger1";
    const std::string table = "TableA";
    auto payloads = RunFunctionColConstQuery("RIGHT", "\"" + text + "\"", "ABS(" + colIntName + ")", table);

    std::vector<std::string> expectedResultsStrings;
    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colIntName).get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            int32_t cutLen = abs(blockInt->GetData()[k]);
            size_t origLen = text.size();
            expectedResultsStrings.push_back(text.substr(origLen < cutLen ? 0 : origLen - cutLen));
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringRightConstConst)
{
    const std::string text = "  ABCDabcdzZ [{|}]_#90  ";
    const int32_t testLen = 2;
    auto payloads = RunFunctionColConstQuery("RIGHT", "\"" + text + "\"", std::to_string(testLen), "TableA LIMIT 1");

    std::vector<std::string> expectedResultsStrings;

    size_t origLen = text.size();
    expectedResultsStrings.push_back(text.substr(origLen < testLen ? 0 : origLen - testLen));

    AssertEqStringCol(payloads, expectedResultsStrings);
}


TEST(DispatcherTests, StringConcatColCol)
{
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    const int32_t testLen = 2;
    auto payloads = RunQuery("CONCAT(" + table + "." + colStrName + "," + table + "." + colStrName + ")",
                             "FROM " + table);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k] + block->GetData()[k]);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringConcatColConst)
{
    const std::string colStrName = "colString1";
    const std::string text = "az#7";
    const std::string table = "TableA";
    const int32_t testLen = 2;
    auto payloads = RunQuery("CONCAT(" + table + "." + colStrName + ",\"" + text + "\")", "FROM " + table);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k] + text);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringConcatConstCol)
{
    const std::string text = "az#7";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    const int32_t testLen = 2;
    auto payloads = RunQuery("CONCAT(\"" + text + "\"," + table + "." + colStrName + ")", "FROM " + table);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(text + block->GetData()[k]);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringConcatConstConst)
{
    const std::string text1 = "abcd";
    const std::string text2 = "XYZ_2";
    const std::string table = "TableA";
    auto payloads = RunQuery("CONCAT(\"" + text1 + "\",\"" + text2 + "\")", "FROM " + table + " LIMIT 1");

    std::vector<std::string> expectedResultsStrings;

    expectedResultsStrings.push_back(text1 + text2);

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringEqColConst)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName,
                             "FROM " + table + " WHERE " + colStrName + " = \"" + text + "\"");

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (block->GetData()[k] == text)
            {
                expectedResultsStrings.push_back(block->GetData()[k]);
            }
        }
    }
    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringNotEqColConst)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName,
                             "FROM " + table + " WHERE " + colStrName + " != \"" + text + "\"");

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (block->GetData()[k] != text)
            {
                expectedResultsStrings.push_back(block->GetData()[k]);
            }
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringEqConstCol)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName,
                             "FROM " + table + " WHERE " + "\"" + text + "\"" + " = " + colStrName);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (block->GetData()[k] == text)
            {
                expectedResultsStrings.push_back(block->GetData()[k]);
            }
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringNotEqConstCol)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName,
                             "FROM " + table + " WHERE " + "\"" + text + "\"" + " != " + colStrName);

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (block->GetData()[k] != text)
            {
                expectedResultsStrings.push_back(block->GetData()[k]);
            }
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringEqConstConst)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName,
                             "FROM " + table + " WHERE " + "\"" + text + "\"" + " = \"" + text + "\"");

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k]);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}

TEST(DispatcherTests, StringNotEqConstConst)
{
    const std::string text = "Word0";
    const std::string colStrName = "colString1";
    const std::string table = "TableA";
    auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + "\"" + text +
                                                           "diff" + "\"" + " != \"" + text + "\"");

    std::vector<std::string> expectedResultsStrings;
    auto columnString = dynamic_cast<ColumnBase<std::string>*>(
        DispatcherObjs::GetInstance().database->GetTables().at(table).GetColumns().at(colStrName).get());

    for (int i = 0; i < 2; i++)
    {
        auto block = columnString->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsStrings.push_back(block->GetData()[k]);
        }
    }

    AssertEqStringCol(payloads, expectedResultsStrings);
}


// Polygon clipping tests
/*
// TODO: fix zero allocation, finish polygon clippin and add asserts
TEST(DispatcherTests, PolygonClippingAndContains)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_,
        "SELECT colInteger1 FROM TableA WHERE GEO_CONTAINS(GEO_INTERSECT(colPolygon1, colPolygon2), colPoint1);");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::string> expectedResultsPoints;
}
*/

TEST(DispatcherTests, CreateDropDatabase)
{
    Context::getInstance();

    GpuSqlCustomParser parser(nullptr, "CREATE DATABASE createdDb;");
    auto resultPtr = parser.Parse();

    ASSERT_TRUE(Database::Exists("createdDb"));

    GpuSqlCustomParser parser2(nullptr, "DROP DATABASE createdDb;");
    resultPtr = parser2.Parse();

    ASSERT_TRUE(!Database::Exists("createdDb"));
}

TEST(DispatcherTests, CreateDropDatabaseWithDelimitedIdentifiers)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "CREATE DATABASE [createdDb%^&*()-+];");
    auto resultPtr = parser.Parse();

    ASSERT_TRUE(Database::Exists("createdDb%^&*()-+"));

    GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database,
                               "DROP DATABASE [createdDb%^&*()-+];");
    resultPtr = parser2.Parse();

    ASSERT_TRUE(!Database::Exists("createdDb%^&*()-+"));
}

TEST(DispatcherTests, CreateAlterDropTable)
{
    Context::getInstance();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") ==
                DispatcherObjs::GetInstance().database->GetTables().end());

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "CREATE TABLE tblA (colA int, colB float, INDEX ind (colA, colB));");
    auto resultPtr = parser.Parse();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") !=
                DispatcherObjs::GetInstance().database->GetTables().end());

    std::vector<std::string> expectedSortingColumns = {"colA", "colB"};
    std::vector<std::string> resultSortingColumns =
        DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetSortingColumns();

    ASSERT_TRUE(expectedSortingColumns.size() == resultSortingColumns.size());

    for (int i = 0; i < expectedSortingColumns.size(); i++)
    {
        ASSERT_TRUE(expectedSortingColumns[i] == resultSortingColumns[i]);
    }

    GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database,
                               "INSERT INTO tblA (colA, colB) VALUES (1, 2.0);");

    for (int32_t i = 0; i < 5; i++)
    {
        resultPtr = parser2.Parse();
    }

    GpuSqlCustomParser parser3(DispatcherObjs::GetInstance().database,
                               "SELECT colA, colB from tblA;");
    resultPtr = parser3.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsColA;
    std::vector<float> expectedResultsColB;

    for (int k = 0; k < 5; k++)
    {
        expectedResultsColA.push_back(1);
        expectedResultsColB.push_back(2.0);
    }

    auto& payloadsColA = result->payloads().at("tblA.colA");
    auto& payloadsColB = result->payloads().at("tblA.colB");

    ASSERT_EQ(payloadsColA.intpayload().intdata_size(), expectedResultsColA.size());

    for (int i = 0; i < payloadsColA.intpayload().intdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColA[i], payloadsColA.intpayload().intdata()[i]);
    }

    ASSERT_EQ(payloadsColB.floatpayload().floatdata_size(), expectedResultsColB.size());

    for (int i = 0; i < payloadsColB.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColB[i], payloadsColB.floatpayload().floatdata()[i]);
    }

    GpuSqlCustomParser parser4(DispatcherObjs::GetInstance().database,
                               "ALTER TABLE tblA DROP COLUMN colA, ADD colC float;");
    resultPtr = parser4.Parse();

    ASSERT_TRUE(
        DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetColumns().find("colA") ==
        DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetColumns().end());


    GpuSqlCustomParser parser5(DispatcherObjs::GetInstance().database,
                               "SELECT colB, colC from tblA;");
    resultPtr = parser5.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloadsColB2 = result->payloads().at("tblA.colB");
    auto& payloadsColC = result->payloads().at("tblA.colC");


    ASSERT_EQ(payloadsColB2.floatpayload().floatdata_size(), expectedResultsColB.size());

    for (int i = 0; i < payloadsColB2.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColB[i], payloadsColB2.floatpayload().floatdata()[i]);
    }

    ASSERT_EQ(payloadsColC.floatpayload().floatdata_size(), 5);

    for (int i = 0; i < payloadsColC.floatpayload().floatdata_size(); i++)
    {
        ASSERT_TRUE(std::isnan(payloadsColC.floatpayload().floatdata()[i]));
    }

    GpuSqlCustomParser parser6(DispatcherObjs::GetInstance().database, "DROP TABLE tblA;");
    resultPtr = parser6.Parse();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") ==
                DispatcherObjs::GetInstance().database->GetTables().end());
}

TEST(DispatcherTests, IsNull)
{
    Context::getInstance();
    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colInteger1 IS NULL;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IsNotNull)
{
    Context::getInstance();
    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE colInteger1 IS NOT NULL;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    std::vector<float> expectedResultsFloat;
    auto column = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                       .database->GetTables()
                                                       .at("TableA")
                                                       .GetColumns()
                                                       .at("colFloat1")
                                                       .get());
    for (int i = 0; i < 2; i++)
    {
        auto block = column->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsFloat.push_back(block->GetData()[k]);
        }
    }
    auto& payload = result->payloads().at("TableA.colFloat1");
    ASSERT_EQ(payload.floatpayload().floatdata_size(), expectedResultsFloat.size());
    for (int i = 0; i < payload.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payload.floatpayload().floatdata()[i]);
    }
}

// TEST(DispatcherTests, WhereEvaluation)
//{
//	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colInteger1 FROM TableA
// WHERE ((colInteger2 != 500) AND (colInteger2 > 1000000)) OR ((colInteger1 >= 150) AND (colInteger1 < -1000000));");
//	auto resultPtr = parser.Parse();
//	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
//
//	FAIL();
//}
//
// TEST(DispatcherTests, WhereEvaluationColColPropagation)
//{
//	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colInteger1 FROM TableA
// WHERE ((colInteger2 > colInteger1) AND (colInteger2 > 1000000)) OR ((colInteger1 >= 150) AND (colInteger1 < -1000000));");
//	auto resultPtr = parser.Parse();
//	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
//
//	FAIL();
//}

std::string GetInsertIntoValuesString(const std::vector<std::vector<int32_t>>& data, int32_t index)
{
    std::string values;
    for (const auto& vctr : data)
    {
        values += (std::to_string(vctr[index]) + ", ");
    }
    return values.substr(0, values.length() - 2);
}

TEST(DispatcherTests, WhereEvaluationAdvanced)
{
    GpuSqlCustomParser parserCreateDatabase(nullptr, "CREATE DATABASE WhereEvalDatabase 10;");
    auto resultPtr = parserCreateDatabase.Parse();

    GpuSqlCustomParser parserCreateTable(Database::GetDatabaseByName("WhereEvalDatabase"),
                                         "CREATE TABLE TableA (ColA INT, ColB INT, ColC INT, ColD "
                                         "INT, INDEX IndA(ColA, ColB, ColC));");
    resultPtr = parserCreateTable.Parse();

    std::vector<int32_t> dataIntA(
        {1, 2, 3, 4, 5, 12, 17, 16, 19, 20, 1, 5, 3, 4, 2, 40, 150, 59, 110, 70});
    std::vector<int32_t> dataIntB({4, 10, 1, 2, 3, 1, 3, 2, 3, 2, 7, 1, 1, 2, 10, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntC({6, 8, 1, 2, 1, 1, 3, 1, 4, 1, 7, 3, 2, 1, 6, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntD(
        {1, 4, 5, 8, 10, 20, 40, 30, 50, 1, 2, 9, 6, 7, 3, 2, 6, 3, 5, 4});

    for (int32_t i = 0; i < dataIntA.size(); i++)
    {
        GpuSqlCustomParser parserInsertInto(
            Database::GetDatabaseByName("WhereEvalDatabase"),
            "INSERT INTO TableA (ColA, ColB, ColC, ColD) VALUES (" +
                GetInsertIntoValuesString({dataIntA, dataIntB, dataIntC, dataIntD}, i) + ");");
        resultPtr = parserInsertInto.Parse();
    }

    std::vector<int32_t> dataIntASorted(
        {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 12, 16, 17, 19, 20, 40, 59, 70, 110, 150});
    std::vector<int32_t> dataIntBSorted(
        {4, 7, 10, 10, 1, 1, 2, 2, 1, 3, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntCSorted(
        {6, 7, 6, 8, 1, 2, 1, 2, 3, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntDSorted(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 1, 2, 3, 4, 5, 6});

    std::shared_ptr<Database> database = Database::GetDatabaseByName("WhereEvalDatabase");
    auto& table = database->GetTables().at("TableA");

    // ColA testing for correct sort
    auto& blocksColA =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColA").get())->GetBlocksList();
    std::vector<int32_t> dataColA;
    for (int i = 0; i < blocksColA.size(); i++)
    {

        for (int j = 0; j < blocksColA[i]->GetSize(); j++)
        {
            dataColA.push_back(blocksColA[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntASorted.size(), dataColA.size());
    for (int i = 0; i < dataColA.size(); i++)
    {
        ASSERT_EQ(dataIntASorted[i], dataColA[i]);
    }

    // ColB testing for correct sort
    auto& blocksColB =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColB").get())->GetBlocksList();
    std::vector<int32_t> dataColB;
    ASSERT_EQ(blocksColB.size(), 4);
    for (int i = 0; i < blocksColB.size(); i++)
    {

        ASSERT_EQ(blocksColB[i]->GetSize(), 5);
        for (int j = 0; j < blocksColB[i]->GetSize(); j++)
        {
            dataColB.push_back(blocksColB[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntBSorted.size(), dataColB.size());
    for (int i = 0; i < dataColB.size(); i++)
    {
        ASSERT_EQ(dataIntBSorted[i], dataColB[i]);
    }

    // ColC testing for correct sort
    auto& blocksColC =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColC").get())->GetBlocksList();
    std::vector<int32_t> dataColC;
    for (int i = 0; i < blocksColC.size(); i++)
    {

        for (int j = 0; j < blocksColC[i]->GetSize(); j++)
        {
            dataColC.push_back(blocksColC[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntCSorted.size(), dataColC.size());
    for (int i = 0; i < dataColC.size(); i++)
    {
        ASSERT_EQ(dataIntCSorted[i], dataColC[i]);
    }

    // ColD testing for correct sort
    auto& blocksColD =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColD").get())->GetBlocksList();
    std::vector<int32_t> dataColD;
    for (int i = 0; i < blocksColD.size(); i++)
    {

        for (int j = 0; j < blocksColD[i]->GetSize(); j++)
        {
            dataColD.push_back(blocksColD[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntDSorted.size(), dataColD.size());
    for (int i = 0; i < dataColD.size(); i++)
    {
        ASSERT_EQ(dataIntDSorted[i], dataColD[i]);
    }

    GpuSqlCustomParser parser(Database::GetDatabaseByName("WhereEvalDatabase"),
                              "SELECT ColA FROM TableA WHERE (ColA >= 10 AND ColA < 50) AND ((5 < "
                              "(ColB + ColC)) AND SIN(ColD));");
    resultPtr = parser.Parse();
    LoadColHelper& loadColHelper = LoadColHelper::getInstance();

    ASSERT_EQ(loadColHelper.countSkippedBlocks, 2);

    GpuSqlCustomParser parserDropDatabase(nullptr, "DROP DATABASE WhereEvalDatabase;");
    resultPtr = parserDropDatabase.Parse();
}

TEST(DispatcherTests, WhereEvaluationAdvanced_FourTimesAnd)
{
    GpuSqlCustomParser parserCreateDatabase(nullptr, "CREATE DATABASE WhereEvalDatabase 10;");
    auto resultPtr = parserCreateDatabase.Parse();

    GpuSqlCustomParser parserCreateTable(Database::GetDatabaseByName("WhereEvalDatabase"),
                                         "CREATE TABLE TableA (ColA INT, ColB INT, ColC INT, ColD "
                                         "INT, INDEX IndA(ColA, ColB, ColC));");
    resultPtr = parserCreateTable.Parse();

    std::vector<int32_t> dataIntA(
        {1, 2, 3, 4, 5, 12, 17, 16, 19, 20, 1, 5, 3, 4, 2, 40, 150, 59, 110, 70});
    std::vector<int32_t> dataIntB({4, 10, 1, 2, 3, 1, 3, 2, 3, 2, 7, 1, 1, 2, 10, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntC({6, 8, 1, 2, 1, 1, 3, 1, 4, 1, 7, 3, 2, 1, 6, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntD(
        {1, 4, 5, 8, 10, 20, 40, 30, 50, 1, 2, 9, 6, 7, 3, 2, 6, 3, 5, 4});

    for (int32_t i = 0; i < dataIntA.size(); i++)
    {
        GpuSqlCustomParser parserInsertInto(
            Database::GetDatabaseByName("WhereEvalDatabase"),
            "INSERT INTO TableA (ColA, ColB, ColC, ColD) VALUES (" +
                GetInsertIntoValuesString({dataIntA, dataIntB, dataIntC, dataIntD}, i) + ");");
        resultPtr = parserInsertInto.Parse();
    }

    std::vector<int32_t> dataIntASorted(
        {1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 12, 16, 17, 19, 20, 40, 59, 70, 110, 150});
    std::vector<int32_t> dataIntBSorted(
        {4, 7, 10, 10, 1, 1, 2, 2, 1, 3, 1, 2, 3, 3, 2, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntCSorted(
        {6, 7, 6, 8, 1, 2, 1, 2, 3, 1, 1, 1, 3, 4, 1, 1, 1, 1, 1, 1});
    std::vector<int32_t> dataIntDSorted(
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50, 1, 2, 3, 4, 5, 6});

    std::shared_ptr<Database> database = Database::GetDatabaseByName("WhereEvalDatabase");
    auto& table = database->GetTables().at("TableA");

    // ColA testing for correct sort
    auto& blocksColA =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColA").get())->GetBlocksList();
    std::vector<int32_t> dataColA;
    for (int i = 0; i < blocksColA.size(); i++)
    {

        for (int j = 0; j < blocksColA[i]->GetSize(); j++)
        {
            dataColA.push_back(blocksColA[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntASorted.size(), dataColA.size());
    for (int i = 0; i < dataColA.size(); i++)
    {
        ASSERT_EQ(dataIntASorted[i], dataColA[i]);
    }

    // ColB testing for correct sort
    auto& blocksColB =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColB").get())->GetBlocksList();
    std::vector<int32_t> dataColB;
    ASSERT_EQ(blocksColB.size(), 4);
    for (int i = 0; i < blocksColB.size(); i++)
    {

        ASSERT_EQ(blocksColB[i]->GetSize(), 5);
        for (int j = 0; j < blocksColB[i]->GetSize(); j++)
        {
            dataColB.push_back(blocksColB[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntBSorted.size(), dataColB.size());
    for (int i = 0; i < dataColB.size(); i++)
    {
        ASSERT_EQ(dataIntBSorted[i], dataColB[i]);
    }

    // ColC testing for correct sort
    auto& blocksColC =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColC").get())->GetBlocksList();
    std::vector<int32_t> dataColC;
    for (int i = 0; i < blocksColC.size(); i++)
    {

        for (int j = 0; j < blocksColC[i]->GetSize(); j++)
        {
            dataColC.push_back(blocksColC[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntCSorted.size(), dataColC.size());
    for (int i = 0; i < dataColC.size(); i++)
    {
        ASSERT_EQ(dataIntCSorted[i], dataColC[i]);
    }

    // ColD testing for correct sort
    auto& blocksColD =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColD").get())->GetBlocksList();
    std::vector<int32_t> dataColD;
    for (int i = 0; i < blocksColD.size(); i++)
    {

        for (int j = 0; j < blocksColD[i]->GetSize(); j++)
        {
            dataColD.push_back(blocksColD[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntDSorted.size(), dataColD.size());
    for (int i = 0; i < dataColD.size(); i++)
    {
        ASSERT_EQ(dataIntDSorted[i], dataColD[i]);
    }

    GpuSqlCustomParser parser(Database::GetDatabaseByName("WhereEvalDatabase"),
                              "SELECT COUNT(ColA) FROM TableA WHERE ColB >= 2 AND ColB <=3 AND "
                              "ColC>=4 AND ColC <= 8 GROUP BY(ColA);");
    resultPtr = parser.Parse();
    LoadColHelper& loadColHelper = LoadColHelper::getInstance();

    ASSERT_EQ(loadColHelper.countSkippedBlocks, 2);

    GpuSqlCustomParser parserDropDatabase(nullptr, "DROP DATABASE WhereEvalDatabase;");
    resultPtr = parserDropDatabase.Parse();
}

TEST(DispatcherTests, WhereEvaluationAdvanced_FilterColCol)
{
    GpuSqlCustomParser parserCreateDatabase(nullptr, "CREATE DATABASE WhereEvalDatabase 10;");
    auto resultPtr = parserCreateDatabase.Parse();

    GpuSqlCustomParser parserCreateTable(Database::GetDatabaseByName("WhereEvalDatabase"),
                                         "CREATE TABLE TableA (ColA INT, ColB INT, INDEX "
                                         "IndA(ColA, ColB));");
    resultPtr = parserCreateTable.Parse();

    std::vector<int32_t> dataIntA({2, 3, 13});
    std::vector<int32_t> dataIntB({1, 5, 10});

    for (int32_t i = 0; i < dataIntA.size(); i++)
    {
        GpuSqlCustomParser parserInsertInto(Database::GetDatabaseByName("WhereEvalDatabase"),
                                            "INSERT INTO TableA (ColA, ColB) VALUES (" +
                                                GetInsertIntoValuesString({dataIntA, dataIntB}, i) + ");");
        resultPtr = parserInsertInto.Parse();
    }

    std::shared_ptr<Database> database = Database::GetDatabaseByName("WhereEvalDatabase");
    auto& table = database->GetTables().at("TableA");

    // ColA testing for correct sort
    auto& blocksColA =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColA").get())->GetBlocksList();
    std::vector<int32_t> dataColA;
    for (int i = 0; i < blocksColA.size(); i++)
    {

        for (int j = 0; j < blocksColA[i]->GetSize(); j++)
        {
            dataColA.push_back(blocksColA[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntA.size(), dataColA.size());
    for (int i = 0; i < dataColA.size(); i++)
    {
        ASSERT_EQ(dataIntA[i], dataColA[i]);
    }

    // ColB testing for correct sort
    auto& blocksColB =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColB").get())->GetBlocksList();
    std::vector<int32_t> dataColB;
    for (int i = 0; i < blocksColB.size(); i++)
    {

        for (int j = 0; j < blocksColB[i]->GetSize(); j++)
        {
            dataColB.push_back(blocksColB[i]->GetData()[j]);
        }
    }

    ASSERT_EQ(dataIntB.size(), dataColB.size());
    for (int i = 0; i < dataColB.size(); i++)
    {
        ASSERT_EQ(dataIntB[i], dataColB[i]);
    }

    GpuSqlCustomParser parser(Database::GetDatabaseByName("WhereEvalDatabase"),
                              "SELECT ColA FROM TableA WHERE ColA <= ColB;");
    resultPtr = parser.Parse();
    LoadColHelper& loadColHelper = LoadColHelper::getInstance();

    ASSERT_EQ(loadColHelper.countSkippedBlocks, 0);

    GpuSqlCustomParser parserDropDatabase(nullptr, "DROP DATABASE WhereEvalDatabase;");
    resultPtr = parserDropDatabase.Parse();
}

template <typename T>
struct IdxKeyPair
{
    int32_t index;
    T key;
};

template <typename T>
struct Asc
{
    inline bool operator()(const IdxKeyPair<T>& struct1, const IdxKeyPair<T>& struct2)
    {
        return (struct1.key < struct2.key);
    }
};

template <typename T>
struct Desc
{
    inline bool operator()(const IdxKeyPair<T>& struct1, const IdxKeyPair<T>& struct2)
    {
        return (struct1.key > struct2.key);
    }
};

TEST(DispatcherTests, OrderByTestSimple)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA ORDER BY colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int32_t i = 0; i < result->payloads().at("TableA.colInteger1").intpayload().intdata_size(); i++)
        expectedResultsInt.push_back(result->payloads().at("TableA.colInteger1").intpayload().intdata()[i]);

    std::vector<IdxKeyPair<int32_t>> v(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

    for (int i = 0, k = 0; i < TEST_BLOCK_COUNT; i++)
        for (int j = 0; j < TEST_BLOCK_SIZE; j++, k++)
            v[k] = {0, columnInt->GetBlocksList()[i]->GetData()[j]};

    stable_sort(v.begin(), v.end(), Asc<int32_t>());

    for (int i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], v[i].key) << i;
    }
}

TEST(DispatcherTests, OrderByTestMulticolumnMultitype)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1, colDouble1 FROM TableA ORDER BY colInteger1 "
                              "ASC, colLong1 DESC, colFloat1 ASC, colDouble1 DESC;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto dataIn1 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get())
                       ->GetBlocksList();
    auto dataIn2 = dynamic_cast<ColumnBase<int64_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colLong1")
                                                          .get())
                       ->GetBlocksList();
    auto dataIn3 = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                        .database->GetTables()
                                                        .at("TableA")
                                                        .GetColumns()
                                                        .at("colFloat1")
                                                        .get())
                       ->GetBlocksList();
    auto dataIn4 = dynamic_cast<ColumnBase<double>*>(DispatcherObjs::GetInstance()
                                                         .database->GetTables()
                                                         .at("TableA")
                                                         .GetColumns()
                                                         .at("colDouble1")
                                                         .get())
                       ->GetBlocksList();

    // Get the expected results
    std::vector<int32_t> expectedResultsInt;
    std::vector<double> expectedResultsDouble;

    for (int32_t i = 0; i < result->payloads().at("TableA.colInteger1").intpayload().intdata_size(); i++)
        expectedResultsInt.push_back(result->payloads().at("TableA.colInteger1").intpayload().intdata()[i]);

    for (int32_t i = 0; i < result->payloads().at("TableA.colDouble1").doublepayload().doubledata_size(); i++)
        expectedResultsDouble.push_back(
            result->payloads().at("TableA.colDouble1").doublepayload().doubledata()[i]);

    // Temp buffers for sort on the CPU
    std::vector<int32_t> data1(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<int64_t> data2(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<float> data3(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<double> data4(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

    std::vector<IdxKeyPair<int32_t>> v1(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<int64_t>> v2(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<float>> v3(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<double>> v4(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

    std::vector<int32_t> indices(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = i;

    // Sort 4th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data4[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v4[i] = {indices[i], data4[i]};

    stable_sort(v4.begin(), v4.end(), Desc<double>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v4[i].index;

    // Sort 3th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data3[i] = dataIn3[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v3[i] = {indices[i], data3[i]};

    stable_sort(v3.begin(), v3.end(), Asc<float>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v3[i].index;

    // Sort 2th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data2[i] = dataIn2[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v2[i] = {indices[i], data2[i]};

    stable_sort(v2.begin(), v2.end(), Desc<int64_t>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v2[i].index;

    // Sort 1th col
    for (int32_t i = 0; i < TEST_BLOCK_COUNT * TEST_BLOCK_SIZE; i++)
        data1[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v1[i] = {indices[i], data1[i]};

    stable_sort(v1.begin(), v1.end(), Asc<int32_t>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v1[i].index;


    // Reorder the output data
    std::vector<int32_t> resultsInt(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<double> resultsDouble(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
    {
        resultsInt[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
        resultsDouble[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
    }

    // Compare the results with the parser results
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
    {
        ASSERT_EQ(resultsInt[i], expectedResultsInt[i]);
        ASSERT_FLOAT_EQ(resultsDouble[i], expectedResultsDouble[i]);
    }
}

TEST(DispatcherTests, OrderByAliasTestMulticolumnMultitype)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1, colDouble1 FROM TableA ORDER BY 1 "
                              "ASC, colLong1 DESC, colFloat1 ASC, 2 DESC;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto dataIn1 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get())
                       ->GetBlocksList();
    auto dataIn2 = dynamic_cast<ColumnBase<int64_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colLong1")
                                                          .get())
                       ->GetBlocksList();
    auto dataIn3 = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                        .database->GetTables()
                                                        .at("TableA")
                                                        .GetColumns()
                                                        .at("colFloat1")
                                                        .get())
                       ->GetBlocksList();
    auto dataIn4 = dynamic_cast<ColumnBase<double>*>(DispatcherObjs::GetInstance()
                                                         .database->GetTables()
                                                         .at("TableA")
                                                         .GetColumns()
                                                         .at("colDouble1")
                                                         .get())
                       ->GetBlocksList();

    // Get the expected results
    std::vector<int32_t> expectedResultsInt;
    std::vector<double> expectedResultsDouble;

    for (int32_t i = 0; i < result->payloads().at("TableA.colInteger1").intpayload().intdata_size(); i++)
        expectedResultsInt.push_back(result->payloads().at("TableA.colInteger1").intpayload().intdata()[i]);

    for (int32_t i = 0; i < result->payloads().at("TableA.colDouble1").doublepayload().doubledata_size(); i++)
        expectedResultsDouble.push_back(
            result->payloads().at("TableA.colDouble1").doublepayload().doubledata()[i]);

    // Temp buffers for sort on the CPU
    std::vector<int32_t> data1(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<int64_t> data2(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<float> data3(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<double> data4(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

    std::vector<IdxKeyPair<int32_t>> v1(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<int64_t>> v2(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<float>> v3(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<IdxKeyPair<double>> v4(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

    std::vector<int32_t> indices(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = i;

    // Sort 4th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data4[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v4[i] = {indices[i], data4[i]};

    stable_sort(v4.begin(), v4.end(), Desc<double>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v4[i].index;

    // Sort 3th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data3[i] = dataIn3[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v3[i] = {indices[i], data3[i]};

    stable_sort(v3.begin(), v3.end(), Asc<float>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v3[i].index;

    // Sort 2th col
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        data2[i] = dataIn2[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v2[i] = {indices[i], data2[i]};

    stable_sort(v2.begin(), v2.end(), Desc<int64_t>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v2[i].index;

    // Sort 1th col
    for (int32_t i = 0; i < TEST_BLOCK_COUNT * TEST_BLOCK_SIZE; i++)
        data1[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        v1[i] = {indices[i], data1[i]};

    stable_sort(v1.begin(), v1.end(), Asc<int32_t>());

    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = v1[i].index;


    // Reorder the output data
    std::vector<int32_t> resultsInt(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    std::vector<double> resultsDouble(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
    {
        resultsInt[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
        resultsDouble[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
    }

    // Compare the results with the parser results
    for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
    {
        ASSERT_EQ(resultsInt[i], expectedResultsInt[i]);
        ASSERT_FLOAT_EQ(resultsDouble[i], expectedResultsDouble[i]);
    }
}

TEST(DispatcherTests, JoinSimpleTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA JOIN TableB ON colInteger1 = "
                              "colInteger3;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::cout
        << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size()
        << std::endl;

    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    std::vector<int32_t> expectedResults;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        expectedResults.push_back(leftBlock->GetData()[leftRowIdx]);
                    }
                }
            }
        }
    }

    auto payloads = result->payloads().at("TableA.colInteger1");
    std::vector<int32_t> payloadVector(payloads.intpayload().intdata().begin(),
                                       payloads.intpayload().intdata().end());

    std::sort(expectedResults.begin(), expectedResults.end());
    std::sort(payloadVector.begin(), payloadVector.end());

    ASSERT_EQ(payloads.intpayload().intdata().size(), expectedResults.size());

    for (int32_t i = 0; i < expectedResults.size(); i++)
    {
        ASSERT_EQ(expectedResults[i], payloadVector[i]);
    }
}

TEST(DispatcherTests, JoinWhereTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 FROM TableA JOIN TableB ON colInteger1 = "
                              "colInteger3 WHERE colFloat1 < 200;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::cout
        << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size()
        << std::endl;

    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colFloat1")
                                                             .get());

    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    std::vector<int32_t> expectedResults;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        auto leftBlockFloat = leftColFloat->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlockFloat->GetData()[leftRowIdx] < 200 &&
                        leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        expectedResults.push_back(leftBlock->GetData()[leftRowIdx]);
                    }
                }
            }
        }
    }

    auto payloads = result->payloads().at("TableA.colInteger1");
    std::vector<int32_t> payloadVector(payloads.intpayload().intdata().begin(),
                                       payloads.intpayload().intdata().end());

    std::sort(expectedResults.begin(), expectedResults.end());
    std::sort(payloadVector.begin(), payloadVector.end());

    ASSERT_EQ(payloads.intpayload().intdata().size(), expectedResults.size());

    for (int32_t i = 0; i < expectedResults.size(); i++)
    {
        ASSERT_EQ(expectedResults[i], payloadVector[i]);
    }
}

TEST(DispatcherTests, JoinGroupByTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1, COUNT(colInteger1) FROM TableA JOIN TableB ON "
                              "colInteger1 = colInteger3 GROUP BY colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::cout
        << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size()
        << std::endl;

    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colFloat1")
                                                             .get());

    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    std::unordered_map<int32_t, int32_t> expectedResults;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        if (expectedResults.find(leftBlock->GetData()[leftRowIdx]) ==
                            expectedResults.end())
                        {
                            expectedResults.insert({leftBlock->GetData()[leftRowIdx], 1});
                        }
                        else
                        {
                            expectedResults.at(leftBlock->GetData()[leftRowIdx])++;
                        }
                    }
                }
            }
        }
    }

    auto payloadsKeys = result->payloads().at("TableA.colInteger1");
    auto payloadsValues = result->payloads().at("COUNT(colInteger1)");


    ASSERT_EQ(payloadsKeys.intpayload().intdata().size(), expectedResults.size());
    ASSERT_EQ(payloadsValues.int64payload().int64data().size(), expectedResults.size());

    for (int32_t i = 0; i < payloadsKeys.intpayload().intdata().size(); i++)
    {
        ASSERT_TRUE(expectedResults.find(payloadsKeys.intpayload().intdata()[i]) != expectedResults.end());
        ASSERT_EQ(expectedResults.at(payloadsKeys.intpayload().intdata()[i]),
                  payloadsValues.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, JoinGroupByWhereTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1, COUNT(colInteger1) FROM TableA JOIN TableB ON "
                              "colInteger1 = colInteger3 WHERE colFloat1 < 200 GROUP BY "
                              "colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::cout
        << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size()
        << std::endl;

    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colFloat1")
                                                             .get());

    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    std::unordered_map<int32_t, int32_t> expectedResults;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        auto leftBlockFloat = leftColFloat->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlockFloat->GetData()[leftRowIdx] < 200 &&
                        leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        if (expectedResults.find(leftBlock->GetData()[leftRowIdx]) ==
                            expectedResults.end())
                        {
                            expectedResults.insert({leftBlock->GetData()[leftRowIdx], 1});
                        }
                        else
                        {
                            expectedResults.at(leftBlock->GetData()[leftRowIdx])++;
                        }
                    }
                }
            }
        }
    }

    auto payloadsKeys = result->payloads().at("TableA.colInteger1");
    auto payloadsValues = result->payloads().at("COUNT(colInteger1)");


    ASSERT_EQ(payloadsKeys.intpayload().intdata().size(), expectedResults.size());
    ASSERT_EQ(payloadsValues.int64payload().int64data().size(), expectedResults.size());

    for (int32_t i = 0; i < payloadsKeys.intpayload().intdata().size(); i++)
    {
        ASSERT_TRUE(expectedResults.find(payloadsKeys.intpayload().intdata()[i]) != expectedResults.end());
        ASSERT_EQ(expectedResults.at(payloadsKeys.intpayload().intdata()[i]),
                  payloadsValues.int64payload().int64data()[i]);
    }
}

TEST(DispatcherTests, JoinWhereStringTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colString1 FROM TableA JOIN TableB ON colInteger1 = "
                              "colInteger3 WHERE colFloat1 < 200;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::cout << "Result size: "
              << result->payloads().at("TableA.colString1").stringpayload().stringdata().size()
              << std::endl;

    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                             .database->GetTables()
                                                             .at("TableA")
                                                             .GetColumns()
                                                             .at("colFloat1")
                                                             .get());
    auto leftColString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                                    .database->GetTables()
                                                                    .at("TableA")
                                                                    .GetColumns()
                                                                    .at("colString1")
                                                                    .get());

    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    std::vector<std::string> expectedResults;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        auto leftBlockFloat = leftColFloat->GetBlocksList()[leftBlockIdx];
        auto leftBlockString = leftColString->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlockFloat->GetData()[leftRowIdx] < 200 &&
                        leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        expectedResults.push_back(leftBlockString->GetData()[leftRowIdx]);
                    }
                }
            }
        }
    }

    auto payloads = result->payloads().at("TableA.colString1");


    std::vector<std::string> payloadVector(payloads.stringpayload().stringdata().begin(),
                                           payloads.stringpayload().stringdata().end());

    std::sort(expectedResults.begin(), expectedResults.end());
    std::sort(payloadVector.begin(), payloadVector.end());

    ASSERT_EQ(payloads.stringpayload().stringdata().size(), expectedResults.size());

    for (int32_t i = 0; i < expectedResults.size(); i++)
    {
        ASSERT_EQ(expectedResults[i], payloadVector[i]);
    }
}

TEST(DispatcherTests, CreateAlterDropTableWithDelimitedIdentifiers)
{
    Context::getInstance();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") ==
                DispatcherObjs::GetInstance().database->GetTables().end());

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "CREATE TABLE [tblA%^&*()-+] ([colA%^&*()-+] int, [colB%^&*()-+] "
                              "float, INDEX [ind%^&*()-+] ([colA%^&*()-+], [colB%^&*()-+]));");
    auto resultPtr = parser.Parse();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") !=
                DispatcherObjs::GetInstance().database->GetTables().end());

    std::vector<std::string> expectedSortingColumns = {"colA%^&*()-+", "colB%^&*()-+"};
    std::vector<std::string> resultSortingColumns =
        DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetSortingColumns();

    ASSERT_TRUE(expectedSortingColumns.size() == resultSortingColumns.size());

    for (int i = 0; i < expectedSortingColumns.size(); i++)
    {
        ASSERT_TRUE(expectedSortingColumns[i] == resultSortingColumns[i]);
    }

    GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database,
                               "INSERT INTO [tblA%^&*()-+] ([colA%^&*()-+], [colB%^&*()-+]) VALUES "
                               "(1, 2.0);");

    for (int32_t i = 0; i < 5; i++)
    {
        resultPtr = parser2.Parse();
    }

    GpuSqlCustomParser parser3(DispatcherObjs::GetInstance().database,
                               "SELECT [colA%^&*()-+], [colB%^&*()-+] from [tblA%^&*()-+];");
    resultPtr = parser3.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsColA;
    std::vector<float> expectedResultsColB;

    for (int k = 0; k < 5; k++)
    {
        expectedResultsColA.push_back(1);
        expectedResultsColB.push_back(2.0);
    }

    auto& payloadsColA = result->payloads().at("tblA%^&*()-+.colA%^&*()-+");
    auto& payloadsColB = result->payloads().at("tblA%^&*()-+.colB%^&*()-+");

    ASSERT_EQ(payloadsColA.intpayload().intdata_size(), expectedResultsColA.size());

    for (int i = 0; i < payloadsColA.intpayload().intdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColA[i], payloadsColA.intpayload().intdata()[i]);
    }

    ASSERT_EQ(payloadsColB.floatpayload().floatdata_size(), expectedResultsColB.size());

    for (int i = 0; i < payloadsColB.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColB[i], payloadsColB.floatpayload().floatdata()[i]);
    }

    GpuSqlCustomParser parser4(DispatcherObjs::GetInstance().database,
                               "ALTER TABLE [tblA%^&*()-+] DROP COLUMN [colA%^&*()-+], ADD "
                               "[colC%^&*()-+] float;");
    resultPtr = parser4.Parse();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetColumns().find("colA%^&*()-+") ==
                DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetColumns().end());


    GpuSqlCustomParser parser5(DispatcherObjs::GetInstance().database,
                               "SELECT [colB%^&*()-+], [colC%^&*()-+] from [tblA%^&*()-+];");
    resultPtr = parser5.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsColC;

    for (int k = 0; k < 5; k++)
    {
        expectedResultsColC.push_back(0.0);
    }

    auto& payloadsColB2 = result->payloads().at("tblA%^&*()-+.colB%^&*()-+");
    auto& payloadsColC = result->payloads().at("tblA%^&*()-+.colC%^&*()-+");


    ASSERT_EQ(payloadsColB2.floatpayload().floatdata_size(), expectedResultsColB.size());

    for (int i = 0; i < payloadsColB2.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsColB[i], payloadsColB2.floatpayload().floatdata()[i]);
    }

    ASSERT_EQ(payloadsColC.floatpayload().floatdata_size(), expectedResultsColC.size());

    for (int i = 0; i < payloadsColC.floatpayload().floatdata_size(); i++)
    {
        ASSERT_TRUE(std::isnan(payloadsColC.floatpayload().floatdata()[i]));
    }

    GpuSqlCustomParser parser6(DispatcherObjs::GetInstance().database,
                               "DROP TABLE [tblA%^&*()-+];");
    resultPtr = parser6.Parse();

    ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") ==
                DispatcherObjs::GetInstance().database->GetTables().end());
}

//== Cast Tests ==
TEST(DispatcherTests, CastFloatColToInt)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT CAST(colFloat1 AS INT) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockFloat = columnFloat->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsInt.push_back(static_cast<int32_t>(blockFloat->GetData()[k]));
        }
    }

    auto& payloadsInt = result->payloads().at("CAST(colFloat1ASINT)");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, CastIntColToFloat)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT CAST(colInteger1 AS FLOAT) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsFloat.push_back(static_cast<float>(blockInt->GetData()[k]));
        }
    }

    auto& payloadsFloat = result->payloads().at("CAST(colInteger1ASFLOAT)");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(payloadsFloat.floatpayload().floatdata()[i], expectedResultsFloat[i]);
    }
}


TEST(DispatcherTests, AliasWhereSimpleTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1 - 100 AS result FROM TableA WHERE result > 300;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<int32_t> expectedResultsInt;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockInt->GetData()[k] - 100 > 300)
            {
                expectedResultsInt.push_back(blockInt->GetData()[k] - 100);
            }
        }
    }

    auto& payloadsInt = result->payloads().at("result");

    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

    for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(payloadsInt.intpayload().intdata()[i], expectedResultsInt[i]);
    }
}

TEST(DispatcherTests, AllColumnsWithDuplicatesTest)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT *, colInteger1, colFloat1, * FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    ASSERT_EQ(result->payloads().size(),
              DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().size());

    for (auto& column : DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns())
    {
        std::string columnName = column.first;

        switch (column.second->GetColumnType())
        {
        case COLUMN_INT:
        {
            auto col = dynamic_cast<ColumnBase<int32_t>*>(column.second.get());
            std::vector<int32_t> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(block->GetData()[k]);
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);

            ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResults.size());

            for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
            {
                ASSERT_EQ(payloads.intpayload().intdata()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_LONG:
        {
            auto col = dynamic_cast<ColumnBase<int64_t>*>(column.second.get());
            std::vector<int64_t> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(block->GetData()[k]);
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);

            ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResults.size());

            for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
            {
                ASSERT_EQ(payloads.int64payload().int64data()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_FLOAT:
        {
            auto col = dynamic_cast<ColumnBase<float>*>(column.second.get());
            std::vector<float> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(block->GetData()[k]);
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);

            ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResults.size());

            for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
            {
                ASSERT_EQ(payloads.floatpayload().floatdata()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_DOUBLE:
        {
            auto col = dynamic_cast<ColumnBase<double>*>(column.second.get());
            std::vector<double> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(block->GetData()[k]);
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);

            ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResults.size());

            for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
            {
                ASSERT_EQ(payloads.doublepayload().doubledata()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_POINT:
        {
            auto col = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(column.second.get());
            std::vector<std::string> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(PointFactory::WktFromPoint(block->GetData()[k], true));
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);
            ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResults.size());

            for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
            {
                ASSERT_EQ(payloads.stringpayload().stringdata()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_POLYGON:
        {
            auto col = dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(column.second.get());
            std::vector<std::string> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(ComplexPolygonFactory::WktFromPolygon(block->GetData()[k], true));
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);
            ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResults.size());

            for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
            {
                ASSERT_EQ(payloads.stringpayload().stringdata()[i], expectedResults[i]);
            }
        }
        break;
        case COLUMN_STRING:
        {
            auto col = dynamic_cast<ColumnBase<std::string>*>(column.second.get());
            std::vector<std::string> expectedResults;

            for (int i = 0; i < col->GetBlockCount(); i++)
            {
                auto block = col->GetBlocksList()[i];
                for (int k = 0; k < block->GetSize(); k++)
                {
                    expectedResults.push_back(block->GetData()[k]);
                }
            }
            auto& payloads = result->payloads().at("TableA." + columnName);
            ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResults.size());

            for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
            {
                ASSERT_EQ(payloads.stringpayload().stringdata()[i], expectedResults[i]);
            }
        }
        break;
        default:
            break;
        }
    }
}

//== String Where Evaluation ==
TEST(DispatcherTests, StringLeftWhereColConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE LEFT(colString1, 4) = \"Word\";");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                                   .database->GetTables()
                                                                   .at("TableA")
                                                                   .GetColumns()
                                                                   .at("colString1")
                                                                   .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockString = columnString->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if (blockString->GetData()[k].substr(0, 4) == "Word")
            {
                expectedResultsFloat.push_back(blockFloat->GetData()[k]);
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, StringConcatLeftWhereColConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colFloat1 FROM TableA WHERE CONCAT(\"Concat\", "
                              "LEFT(colString1, 4)) = \"ConcatWord\";");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<float> expectedResultsFloat;

    auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                                   .database->GetTables()
                                                                   .at("TableA")
                                                                   .GetColumns()
                                                                   .at("colString1")
                                                                   .get());
    auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colFloat1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockString = columnString->GetBlocksList()[i];
        auto blockFloat = columnFloat->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            if ("Concat" + blockString->GetData()[k].substr(0, 4) == "ConcatWord")
            {
                expectedResultsFloat.push_back(blockFloat->GetData()[k]);
            }
        }
    }

    auto& payloadsFloat = result->payloads().at("TableA.colFloat1");

    ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

    for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
    }
}

TEST(DispatcherTests, RetConst)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    const int32_t expectedSize = 2 * (1 << 11);
    auto& payloadsInt = result->payloads().at("5");
    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedSize);
    for (int i = 0; i < expectedSize; i++)
    {
        ASSERT_EQ(5, payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, RetConstWithFilter)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT 5 FROM TableA WHERE 500 < colInteger1;;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    int32_t expectedSize = 0;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < (1 << 11); j++)
        {
            if (j % 2)
            {
                if (500 < j % 1024)
                {
                    ++expectedSize;
                }
            }
            else
            {
                if (500 < (j % 1024) * -1)
                {
                    ++expectedSize;
                }
            }
        }
    }
    auto& payloadsInt = result->payloads().at("5");
    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedSize);
    for (int i = 0; i < expectedSize; i++)
    {
        ASSERT_EQ(5, payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, RetConstWithColumn)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colInteger1, 5 FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    const int32_t expectedSize = 2 * (1 << 11);
    auto& payloadsInt = result->payloads().at("5");
    ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedSize);
    for (int i = 0; i < expectedSize; i++)
    {
        ASSERT_EQ(5, payloadsInt.intpayload().intdata()[i]);
    }
}

TEST(DispatcherTests, RetConstString)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT \"test\" FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    const int32_t expectedSize = 2 * (1 << 11);
    auto& payloadsStr = result->payloads().at("\"test\"");
    ASSERT_EQ(payloadsStr.stringpayload().stringdata_size(), expectedSize);
    for (int i = 0; i < expectedSize; i++)
    {
        ASSERT_EQ("test", payloadsStr.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, RetConstJoin)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT 5 FROM TableA JOIN TableB ON colInteger1 = "
                              "colInteger3;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());


    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    int32_t expectedResultCount = 0;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
                    {
                        expectedResultCount++;
                    }
                }
            }
        }
    }

    auto payloads = result->payloads().at("5");

    ASSERT_EQ(payloads.intpayload().intdata().size(), expectedResultCount);

    for (int32_t i = 0; i < expectedResultCount; i++)
    {
        ASSERT_EQ(payloads.intpayload().intdata()[i], 5);
    }
}

TEST(DispatcherTests, ReorderStringOrderBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colString1 FROM TableA ORDER BY colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance()
                                                                   .database->GetTables()
                                                                   .at("TableA")
                                                                   .GetColumns()
                                                                   .at("colString1")
                                                                   .get());
    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    std::vector<std::pair<std::string, std::int32_t>> intStringPairs;

    for (int i = 0; i < 2; i++)
    {
        auto blockString = columnString->GetBlocksList()[i];
        auto blockInt = columnInt->GetBlocksList()[i];

        for (int k = 0; k < (1 << 11); k++)
        {
            intStringPairs.push_back({blockString->GetData()[k], blockInt->GetData()[k]});
        }
    }

    std::stable_sort(intStringPairs.begin(), intStringPairs.end(),
                     [](const std::pair<std::string, std::int32_t>& a,
                        const std::pair<std::string, std::int32_t>& b) -> bool {
                         return a.second < b.second;
                     });

    auto& payloadsString = result->payloads().at("TableA.colString1");
    ASSERT_EQ(payloadsString.stringpayload().stringdata_size(), intStringPairs.size());

    for (int i = 0; i < payloadsString.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(intStringPairs[i].first, payloadsString.stringpayload().stringdata()[i]);
    }
}

TEST(DispatcherTests, ReorderPolygonOrderBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPolygon1 FROM TableA ORDER BY colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::pair<std::string, int32_t>> expectedResultsPolygons;

    auto columnPolygon =
        dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(DispatcherObjs::GetInstance()
                                                                        .database->GetTables()
                                                                        .at("TableA")
                                                                        .GetColumns()
                                                                        .at("colPolygon1")
                                                                        .get());

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockPolygon = columnPolygon->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsPolygons.push_back(
                {ComplexPolygonFactory::WktFromPolygon(blockPolygon->GetData()[k], true),
                 blockInt->GetData()[k]});
        }
    }

    std::stable_sort(expectedResultsPolygons.begin(), expectedResultsPolygons.end(),
                     [](const std::pair<std::string, std::int32_t>& a,
                        const std::pair<std::string, std::int32_t>& b) -> bool {
                         return a.second < b.second;
                     });

    auto& payloads = result->payloads().at("TableA.colPolygon1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPolygons.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPolygons[i].first, payloads.stringpayload().stringdata()[i]) << i;
    }
}

TEST(DispatcherTests, ReorderPointOrderBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT colPoint1 FROM TableA ORDER BY colInteger1;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    std::vector<std::pair<std::string, int32_t>> expectedResultsPoints;

    auto columnPoint = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance()
                                                                              .database->GetTables()
                                                                              .at("TableA")
                                                                              .GetColumns()
                                                                              .at("colPoint1")
                                                                              .get());

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < 2; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        auto blockPoint = columnPoint->GetBlocksList()[i];
        for (int k = 0; k < (1 << 11); k++)
        {
            expectedResultsPoints.push_back(
                {PointFactory::WktFromPoint(blockPoint->GetData()[k], true), blockInt->GetData()[k]});
        }
    }

    std::stable_sort(expectedResultsPoints.begin(), expectedResultsPoints.end(),
                     [](const std::pair<std::string, std::int32_t>& a,
                        const std::pair<std::string, std::int32_t>& b) -> bool {
                         return a.second < b.second;
                     });

    auto& payloads = result->payloads().at("TableA.colPoint1");

    ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

    for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsPoints[i].first, payloads.stringpayload().stringdata()[i]) << i;
    }
}

TEST(DispatcherTests, AggregationCountAsteriskNoGroupBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT COUNT(*) FROM TableA;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("COUNT(*)");

    ASSERT_EQ(payloads.int64payload().int64data_size(), 1);
    ASSERT_EQ(payloads.int64payload().int64data()[0], TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
}

TEST(DispatcherTests, AggregationCountAsteriskWhereNoGroupBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT COUNT(*) FROM TableA WHERE colInteger1 > 512;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto& payloads = result->payloads().at("COUNT(*)");

    int64_t outSize = 0;

    auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                            .database->GetTables()
                                                            .at("TableA")
                                                            .GetColumns()
                                                            .at("colInteger1")
                                                            .get());

    for (int i = 0; i < TEST_BLOCK_COUNT; i++)
    {
        auto blockInt = columnInt->GetBlocksList()[i];
        for (int k = 0; k < blockInt->GetSize(); k++)
        {
            if (blockInt->GetData()[k] > 512)
            {
                outSize++;
            }
        }
    }

    ASSERT_EQ(payloads.int64payload().int64data_size(), 1);
    ASSERT_EQ(payloads.int64payload().int64data()[0], outSize);
}

TEST(DispatcherTests, AggregationCountAsterisJoinWhereNoGroupBy)
{
    Context::getInstance();

    GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
                              "SELECT COUNT(*) FROM TableA JOIN TableB ON colInteger1 = "
                              "colInteger3 WHERE colInteger1 > 512;");
    auto resultPtr = parser.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());


    auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                          .database->GetTables()
                                                          .at("TableA")
                                                          .GetColumns()
                                                          .at("colInteger1")
                                                          .get());
    auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance()
                                                           .database->GetTables()
                                                           .at("TableB")
                                                           .GetColumns()
                                                           .at("colInteger3")
                                                           .get());

    int32_t expectedResultCount = 0;

    for (int32_t leftBlockIdx = 0; leftBlockIdx < leftCol->GetBlockCount(); leftBlockIdx++)
    {
        auto leftBlock = leftCol->GetBlocksList()[leftBlockIdx];
        for (int32_t leftRowIdx = 0; leftRowIdx < leftBlock->GetSize(); leftRowIdx++)
        {
            for (int32_t rightBlockIdx = 0; rightBlockIdx < rightCol->GetBlockCount(); rightBlockIdx++)
            {
                auto rightBlock = rightCol->GetBlocksList()[rightBlockIdx];
                for (int32_t rightRowIdx = 0; rightRowIdx < rightBlock->GetSize(); rightRowIdx++)
                {
                    if (leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx] &&
                        leftBlock->GetData()[leftRowIdx] > 512)
                    {
                        expectedResultCount++;
                    }
                }
            }
        }
    }

    auto payloads = result->payloads().at("COUNT(*)");

    ASSERT_EQ(payloads.int64payload().int64data().size(), 1);
    ASSERT_EQ(payloads.int64payload().int64data()[0], expectedResultCount);
}

TEST(DispatcherTests, AlterTableAlterColumnIntToFloat)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterIntToFloat 10;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterIntToFloat");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (col int);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("col")->GetColumnType();

    ASSERT_EQ(type, COLUMN_INT);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (col) VALUES (1);");

    std::vector<float> expectedResultsCol;
    for (int32_t i = 0; i < 22; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultsCol.push_back(static_cast<float>(1));
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN col float;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_FLOAT);

    GpuSqlCustomParser parser4(database, "SELECT col from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.col");

    ASSERT_EQ(payloadsCol.floatpayload().floatdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.floatpayload().floatdata_size(); i++)
    {
        ASSERT_FLOAT_EQ(expectedResultsCol[i], payloadsCol.floatpayload().floatdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterIntToFloat;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnPointToString)
{
    GpuSqlCustomParser createDatabase(nullptr,
                                      "CREATE DATABASE TestDatabaseAlterPointToString 10;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterPointToString");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (col geo_point);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POINT);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (col) VALUES (POINT(10.11 11.1));");

    std::vector<std::string> expectedResultsCol;
    for (int32_t i = 0; i < 22; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultsCol.push_back("POINT(10.11 11.1)");
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN col string;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT col from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.col");

    ASSERT_EQ(payloadsCol.stringpayload().stringdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.stringpayload().stringdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterPointToString;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnPolygonToString)
{
    GpuSqlCustomParser createDatabase(nullptr,
                                      "CREATE DATABASE TestDatabaseAlterPolygonToString 10;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterPolygonToString");
    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (col geo_polygon);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POLYGON);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (col) VALUES (POLYGON((10 11, "
                                         "11.11 12.13, 10 11),(21 30, 35.55 36, 30.11 20.26, 21 "
                                         "30),(61 80.11,90 89.15,112.12 110, 61 80.11)));");

    std::vector<std::string> expectedResultsCol;
    for (int32_t i = 0; i < 22; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultsCol.push_back("POLYGON((10 11, 11.11 12.13, 10 11), (21 30, 35.55 36, 30.11 "
                                     "20.26, 21 30), (61 80.11, 90 89.15, 112.12 110, 61 80.11))");
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN col string;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT col from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.col");

    ASSERT_EQ(payloadsCol.stringpayload().stringdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.stringpayload().stringdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterPolygonToString;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnStringToPolygon)
{
    GpuSqlCustomParser createDatabase(nullptr,
                                      "CREATE DATABASE TestDatabaseAlterStringToPolygon 3;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterStringToPolygon");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colP string, colS string);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser2(database,
                               "INSERT INTO testTable (colP, colS) VALUES (\"POLYGON((10 11, 11.11 "
                               "12.13, 10 11),(21 30, 35.55 36, 30.11 20.26, 21 30),(61 80.11,90 "
                               "89.15,112.12 110, 61 80.11))\", \"randomString\");");

    std::vector<std::string> expectedResultsCol;
    std::vector<std::string> expectedResultsColString;
    for (int32_t i = 0; i < 7; i++)
    {
        resultPtr = parser2.Parse();
        ColmnarDB::Types::ComplexPolygon polygon = ComplexPolygonFactory::FromWkt(
            "POLYGON((10 11, 11.11 12.13, 10 11), (21 30, 35.55 36, 30.11 20.26, 21 30), (61 "
            "80.11, 90 89.15, 112.12 110, 61 80.11))");
        ColmnarDB::Types::ComplexPolygon emptyPolygon =
            ComplexPolygonFactory::FromWkt("POLYGON((0 0, 0 0))");
        expectedResultsCol.push_back(ComplexPolygonFactory::WktFromPolygon(polygon, true));
        expectedResultsColString.push_back(ComplexPolygonFactory::WktFromPolygon(emptyPolygon, true));
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN colP geo_polygon;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POLYGON);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT colP from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.colP");

    ASSERT_EQ(payloadsCol.stringpayload().stringdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.stringpayload().stringdata()[i]);
    }
    ///////
    GpuSqlCustomParser parser5(database, "ALTER TABLE testTable ALTER COLUMN colS geo_polygon;");
    resultPtr = parser5.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POLYGON);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POLYGON);

    GpuSqlCustomParser parser6(database, "SELECT colS from testTable;");
    resultPtr = parser6.Parse();
    auto resultString =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColString = resultString->payloads().at("testTable.colS");

    ASSERT_EQ(payloadsColString.stringpayload().stringdata_size(), expectedResultsColString.size());

    for (int i = 0; i < payloadsColString.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsColString[i], payloadsColString.stringpayload().stringdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterStringToPolygon;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnStringToPoint)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterStringToPoint 3;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterStringToPoint");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colP string, colS string);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colP, colS) VALUES (\"POINT(11.5 "
                                         "1.3)\", \"randomString\");");

    std::vector<std::string> expectedResultsCol;
    std::vector<std::string> expectedResultsColString;
    for (int32_t i = 0; i < 7; i++)
    {
        resultPtr = parser2.Parse();
        ColmnarDB::Types::Point point = PointFactory::FromWkt("POINT(11.5 1.3)");
        ColmnarDB::Types::Point emptyPoint = ColmnarDB::Types::Point();
        expectedResultsCol.push_back(PointFactory::WktFromPoint(point, true));
        expectedResultsColString.push_back(PointFactory::WktFromPoint(emptyPoint, true));
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN colP geo_point;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POINT);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT colP from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.colP");

    ASSERT_EQ(payloadsCol.stringpayload().stringdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.stringpayload().stringdata()[i]);
    }
    ///////
    GpuSqlCustomParser parser5(database, "ALTER TABLE testTable ALTER COLUMN colS geo_point;");
    resultPtr = parser5.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POINT);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POINT);

    GpuSqlCustomParser parser6(database, "SELECT colS from testTable;");
    resultPtr = parser6.Parse();
    auto resultString =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColString = resultString->payloads().at("testTable.colS");

    ASSERT_EQ(payloadsColString.stringpayload().stringdata_size(), expectedResultsColString.size());

    for (int i = 0; i < payloadsColString.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsColString[i], payloadsColString.stringpayload().stringdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterStringToPoint;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnStringToDouble)
{
    GpuSqlCustomParser createDatabase(nullptr,
                                      "CREATE DATABASE TestDatabaseAlterStringToDouble 3;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterStringToDouble");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colP string, colS string);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colP, colS) VALUES (\"2.5\", "
                                         "\"randomString\");");

    std::vector<double> expectedResultsCol;
    // std::vector<double> expectedResultsColString;
    for (int32_t i = 0; i < 7; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultsCol.push_back(2.5);
        // expectedResultsColString.push_back(std::numeric_limits<double>::quiet_NaN());
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN colP double;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_DOUBLE);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT colP from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.colP");

    ASSERT_EQ(payloadsCol.doublepayload().doubledata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.doublepayload().doubledata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.doublepayload().doubledata()[i]);
    }
    ///////
    GpuSqlCustomParser parser5(database, "ALTER TABLE testTable ALTER COLUMN colS double;");
    resultPtr = parser5.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_DOUBLE);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_DOUBLE);

    GpuSqlCustomParser parser6(database, "SELECT colS from testTable;");
    resultPtr = parser6.Parse();
    auto resultString =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColString = resultString->payloads().at("testTable.colS");

    for (int i = 0; i < payloadsColString.doublepayload().doubledata_size(); i++)
    {
        ASSERT_TRUE(std::isnan(payloadsColString.doublepayload().doubledata()[i]));
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterStringToDouble;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnStringToInt)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterStringToInt 3;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterStringToInt");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colP string, colS string);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colP, colS) VALUES (\"2\", "
                                         "\"randomString\");");

    std::vector<int32_t> expectedResultsCol;
    std::vector<int32_t> expectedResultsColString;
    for (int32_t i = 0; i < 7; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultsCol.push_back(2);
        expectedResultsColString.push_back(std::numeric_limits<int32_t>::min());
    }

    GpuSqlCustomParser parser3(database, "ALTER TABLE testTable ALTER COLUMN colP int;");
    resultPtr = parser3.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parser4(database, "SELECT colP from testTable;");
    resultPtr = parser4.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsCol = result->payloads().at("testTable.colP");

    ASSERT_EQ(payloadsCol.intpayload().intdata_size(), expectedResultsCol.size());

    for (int i = 0; i < payloadsCol.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsCol[i], payloadsCol.intpayload().intdata()[i]);
    }
    ///////
    GpuSqlCustomParser parser5(database, "ALTER TABLE testTable ALTER COLUMN colS int;");
    resultPtr = parser5.Parse();

    type = table.GetColumns().at("colP")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);

    type = table.GetColumns().at("colS")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);

    GpuSqlCustomParser parser6(database, "SELECT colS from testTable;");
    resultPtr = parser6.Parse();
    auto resultString =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColString = resultString->payloads().at("testTable.colS");

    ASSERT_EQ(payloadsColString.intpayload().intdata_size(), expectedResultsColString.size());

    for (int i = 0; i < payloadsColString.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultsColString[i], payloadsColString.intpayload().intdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterStringToInt;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnBitmaskCopy)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterBitmaskCopy 10;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterBitmaskCopy");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (col int);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("col")->GetColumnType();

    ASSERT_EQ(type, COLUMN_INT);

    GpuSqlCustomParser parser2(database, "INSERT INTO testTable (col) VALUES (1);");
    GpuSqlCustomParser parser3(database, "INSERT INTO testTable (col) VALUES (NULL);");

    std::vector<float> expectedResultsCol;
    for (int32_t i = 0; i < 11; i++)
    {
        resultPtr = parser2.Parse();
        resultPtr = parser3.Parse();
    }

    GpuSqlCustomParser parserSelect(database, "SELECT col from testTable;");
    resultPtr = parserSelect.Parse();
    auto resultInt =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColInt = resultInt->payloads().at("testTable.col");
    ASSERT_EQ(payloadsColInt.intpayload().intdata_size(), 22);

    for (int i = 0; i < 11; i += 2)
    {
        ASSERT_EQ(1, payloadsColInt.intpayload().intdata()[i]);
    }

    auto blocksBeforeCast =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("col").get())->GetBlocksList();

    std::vector<std::unique_ptr<int8_t[]>> oldBitmasks;

    for (int32_t i = 0; i < blocksBeforeCast.size(); i++)
    {
        size_t bitmaskSize = blocksBeforeCast[i]->GetNullBitmaskSize();
        std::unique_ptr<int8_t[]> bitmask = std::make_unique<int8_t[]>(bitmaskSize);
        std::copy(blocksBeforeCast[i]->GetNullBitmask(),
                  blocksBeforeCast[i]->GetNullBitmask() + bitmaskSize, bitmask.get());
        oldBitmasks.push_back(std::move(bitmask));
    }

    GpuSqlCustomParser parser4(database, "ALTER TABLE testTable ALTER COLUMN col float;");
    resultPtr = parser4.Parse();

    type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_FLOAT);

    auto blocksAfterCast =
        dynamic_cast<ColumnBase<float>*>(table.GetColumns().at("col").get())->GetBlocksList();

    for (int32_t i = 0; i < blocksAfterCast.size(); i++)
    {
        for (int32_t j = 0; j < blocksAfterCast[i]->GetSize(); j++)
        {
            int bitMaskIdx = (j / (sizeof(char) * 8));
            int shiftIdx = (j % (sizeof(char) * 8));

            ASSERT_EQ((oldBitmasks[i][bitMaskIdx] >> shiftIdx) & 1,
                      (blocksAfterCast[i]->GetNullBitmask()[bitMaskIdx] >> shiftIdx) & 1);
        }
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterBitmaskCopy;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAlterColumnBitmaskCopyWithInsertNull)
{
    GpuSqlCustomParser createDatabase(nullptr,
                                      "CREATE DATABASE TestDatabaseAlterBitmaskCopyNull 10;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterBitmaskCopyNull");
    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (col string);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("col")->GetColumnType();

    ASSERT_EQ(type, COLUMN_STRING);

    GpuSqlCustomParser parserValue(database, "INSERT INTO testTable (col) VALUES (\"1\");");
    GpuSqlCustomParser parserNull(database, "INSERT INTO testTable (col) VALUES (NULL);");
    GpuSqlCustomParser parserWrongValue(database,
                                        "INSERT INTO testTable (col) VALUES (\"randomString\");");

    for (int32_t i = 0; i < 5; i++)
    {
        resultPtr = parserValue.Parse();
        resultPtr = parserNull.Parse();
        resultPtr = parserWrongValue.Parse();
    }

    GpuSqlCustomParser parserSelect(database, "SELECT col from testTable;");
    resultPtr = parserSelect.Parse();
    auto resultInt =
        dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto blocksBeforeCast =
        dynamic_cast<ColumnBase<std::string>*>(table.GetColumns().at("col").get())->GetBlocksList();

    std::vector<int8_t> oldBitmasks;
    for (int32_t i = 0; i < 5; i++)
    {
        oldBitmasks.push_back(0);
        oldBitmasks.push_back(1);
        oldBitmasks.push_back(1);
    }

    GpuSqlCustomParser parser4(database, "ALTER TABLE testTable ALTER COLUMN col int;");
    resultPtr = parser4.Parse();

    type = table.GetColumns().at("col")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);

    auto blocksAfterCast =
        dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("col").get())->GetBlocksList();

    std::vector<int8_t> newBitmasks;
    for (int32_t i = 0; i < blocksAfterCast.size(); i++)
    {
        for (int32_t j = 0; j < blocksAfterCast[i]->GetSize(); j++)
        {
            int bitMaskIdx = (j / (sizeof(char) * 8));
            int shiftIdx = (j % (sizeof(char) * 8));
            newBitmasks.push_back((blocksAfterCast[i]->GetNullBitmask()[bitMaskIdx] >> shiftIdx) & 1);
        }
    }

    ASSERT_EQ(oldBitmasks.size(), newBitmasks.size());
    for (int32_t i = 0; i < oldBitmasks.size(); i++)
    {
        ASSERT_EQ(oldBitmasks[i], newBitmasks[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterBitmaskCopyNull;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, ClusteredIndexPoint)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabasePoint 8;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabasePoint");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());


    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colA int, colB geo_point, INDEX "
                                        "ind (colA));");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colA")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);
    type = table.GetColumns().at("colB")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POINT);

    GpuSqlCustomParser parser2(database,
                               "INSERT INTO testTable (colA, colB) VALUES (1, POINT(2.5 3.23));");

    std::vector<int32_t> expectedResultInt;
    std::vector<std::string> expectedResultPoint;
    for (int32_t i = 0; i < 7; i++)
    {
        resultPtr = parser2.Parse();
        expectedResultInt.push_back(1);
        ColmnarDB::Types::Point point = PointFactory::FromWkt("POINT(2.5 3.23)");
        expectedResultPoint.push_back(PointFactory::WktFromPoint(point, true));
    }

    // SELECT COL INT
    GpuSqlCustomParser parserSelectFromA(database, "SELECT colA from testTable;");
    resultPtr = parserSelectFromA.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColInt = result->payloads().at("testTable.colA");

    ASSERT_EQ(payloadsColInt.intpayload().intdata_size(), expectedResultInt.size());

    for (int i = 0; i < payloadsColInt.intpayload().intdata_size(); i++)
    {
        ASSERT_EQ(expectedResultInt[i], payloadsColInt.intpayload().intdata()[i]);
    }

    // SELECT COL POINT
    GpuSqlCustomParser parserSelectFromB(database, "SELECT colB from testTable;");
    resultPtr = parserSelectFromB.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColPoint = result->payloads().at("testTable.colB");

    ASSERT_EQ(payloadsColPoint.stringpayload().stringdata_size(), expectedResultPoint.size());

    for (int i = 0; i < payloadsColPoint.stringpayload().stringdata_size(); i++)
    {
        ASSERT_EQ(expectedResultPoint[i], payloadsColPoint.stringpayload().stringdata()[i]);
    }

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabasePoint;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, ClusteredIndexPolygon)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabasePolygon 4;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabasePolygon");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());


    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colA int, colB geo_polygon, INDEX "
                                        "ind (colA));");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colA")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);
    type = table.GetColumns().at("colB")->GetColumnType();
    ASSERT_EQ(type, COLUMN_POLYGON);

    std::vector<int32_t> expectedResultInt;
    std::vector<std::string> expectedResultPolygon;
    ColmnarDB::Types::ComplexPolygon polygon =
        ComplexPolygonFactory::FromWkt("POLYGON((10 11, 11.11 "
                                       "12.13, 10 11),(21 30, 35.55 36, 30.11 20.26, 21 "
                                       "30),(61 80.11,90 "
                                       "89.15,112.12 110, 61 80.11))");
    for (int32_t i = 0; i < 7; i++)
    {
        GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colA, colB) VALUES (" + std::to_string(i) +
                                                 ", POLYGON((10 11, 11.11 "
                                                 "12.13, 10 11),(21 30, 35.55 36, 30.11 20.26, 21 "
                                                 "30),(61 80.11,90 "
                                                 "89.15,112.12 110, 61 80.11)));");
        resultPtr = parser2.Parse();
        expectedResultInt.push_back(i);
        expectedResultPolygon.push_back(ComplexPolygonFactory::WktFromPolygon(polygon, true));
    }

    // SELECT COL INT
    GpuSqlCustomParser parserSelectFromA(database, "SELECT colA from testTable;");
    resultPtr = parserSelectFromA.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColInt = result->payloads().at("testTable.colA");

    ASSERT_EQ(payloadsColInt.intpayload().intdata_size(), expectedResultInt.size());
    auto& columnIntA = table.GetColumns().at("colA");
    auto& blocksA = dynamic_cast<ColumnBase<int32_t>*>(columnIntA.get())->GetBlocksList();
    ASSERT_EQ(blocksA[0]->GetData()[0], expectedResultInt[0]);
    ASSERT_EQ(blocksA[0]->GetData()[1], expectedResultInt[1]);
    ASSERT_EQ(blocksA[1]->GetData()[0], expectedResultInt[2]);
    ASSERT_EQ(blocksA[1]->GetData()[1], expectedResultInt[3]);
    ASSERT_EQ(blocksA[2]->GetData()[0], expectedResultInt[4]);
    ASSERT_EQ(blocksA[2]->GetData()[1], expectedResultInt[5]);
    ASSERT_EQ(blocksA[2]->GetData()[2], expectedResultInt[6]);


    // SELECT COL POLYGON
    GpuSqlCustomParser parserSelectFromB(database, "SELECT colB from testTable;");
    resultPtr = parserSelectFromB.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColPolygon = result->payloads().at("testTable.colB");

    ASSERT_EQ(payloadsColPolygon.stringpayload().stringdata_size(), expectedResultPolygon.size());
    auto& columnIntB = table.GetColumns().at("colB");
    auto& blocksB =
        dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(columnIntB.get())->GetBlocksList();
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[0]->GetData()[0], true), expectedResultPolygon[0]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[0]->GetData()[1], true), expectedResultPolygon[1]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[1]->GetData()[0], true), expectedResultPolygon[2]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[1]->GetData()[1], true), expectedResultPolygon[3]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[2]->GetData()[0], true), expectedResultPolygon[4]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[2]->GetData()[1], true), expectedResultPolygon[5]);
    ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(blocksB[2]->GetData()[2], true), expectedResultPolygon[6]);

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabasePolygon;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, ClusteredIndexString)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseString 4;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseString");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());


    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colA int, colB string, INDEX "
                                        "ind (colA));");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    auto& table = database->GetTables().at("testTable");
    auto type = table.GetColumns().at("colA")->GetColumnType();
    ASSERT_EQ(type, COLUMN_INT);
    type = table.GetColumns().at("colB")->GetColumnType();
    ASSERT_EQ(type, COLUMN_STRING);

    std::vector<int32_t> expectedResultInt;
    std::vector<std::string> expectedResultString;
    for (int32_t i = 0; i < 7; i++)
    {
        GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colA, colB) VALUES (" +
                                                 std::to_string(i) + ", \"abc" + std::to_string(i) + "\");");
        resultPtr = parser2.Parse();
        expectedResultInt.push_back(i);
        expectedResultString.push_back("abc" + std::to_string(i));
    }

    // SELECT COL INT
    GpuSqlCustomParser parserSelectFromA(database, "SELECT colA from testTable;");
    resultPtr = parserSelectFromA.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColInt = result->payloads().at("testTable.colA");

    ASSERT_EQ(payloadsColInt.intpayload().intdata_size(), expectedResultInt.size());
    auto& columnIntA = table.GetColumns().at("colA");
    auto& blocksA = dynamic_cast<ColumnBase<int32_t>*>(columnIntA.get())->GetBlocksList();
    ASSERT_EQ(blocksA[0]->GetData()[0], expectedResultInt[0]);
    ASSERT_EQ(blocksA[0]->GetData()[1], expectedResultInt[1]);
    ASSERT_EQ(blocksA[1]->GetData()[0], expectedResultInt[2]);
    ASSERT_EQ(blocksA[1]->GetData()[1], expectedResultInt[3]);
    ASSERT_EQ(blocksA[2]->GetData()[0], expectedResultInt[4]);
    ASSERT_EQ(blocksA[2]->GetData()[1], expectedResultInt[5]);
    ASSERT_EQ(blocksA[2]->GetData()[2], expectedResultInt[6]);


    // SELECT COL STRING
    GpuSqlCustomParser parserSelectFromB(database, "SELECT colB from testTable;");
    resultPtr = parserSelectFromB.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

    auto& payloadsColString = result->payloads().at("testTable.colB");

    ASSERT_EQ(payloadsColString.stringpayload().stringdata_size(), expectedResultString.size());
    auto& columnIntB = table.GetColumns().at("colB");
    auto& blocksB = dynamic_cast<ColumnBase<std::string>*>(columnIntB.get())->GetBlocksList();
    ASSERT_EQ(blocksB[0]->GetData()[0], expectedResultString[0]);
    ASSERT_EQ(blocksB[0]->GetData()[1], expectedResultString[1]);
    ASSERT_EQ(blocksB[1]->GetData()[0], expectedResultString[2]);
    ASSERT_EQ(blocksB[1]->GetData()[1], expectedResultString[3]);
    ASSERT_EQ(blocksB[2]->GetData()[0], expectedResultString[4]);
    ASSERT_EQ(blocksB[2]->GetData()[1], expectedResultString[5]);
    ASSERT_EQ(blocksB[2]->GetData()[2], expectedResultString[6]);

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseString;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAddColumnWithIndex)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterAddWI 8;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterAddWI");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colA int, colB int, INDEX "
                                        "ind(colA, colB));");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    for (int32_t i = 0; i < 15; i++)
    {
        GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colA, colB) VALUES (" +
                                                 std::to_string(i) + ", " + std::to_string(i % 5) + ");");
        resultPtr = parser2.Parse();
    }

    auto& table = database->GetTables().at("testTable");
    auto& columnIntA = table.GetColumns().at("colA");
    auto& blocksA = dynamic_cast<ColumnBase<int32_t>*>(columnIntA.get())->GetBlocksList();

    auto& columnIntB = table.GetColumns().at("colB");
    auto& blocksB = dynamic_cast<ColumnBase<int32_t>*>(columnIntB.get())->GetBlocksList();

    ASSERT_EQ(blocksA[0]->GetSize(), 4);
    ASSERT_EQ(blocksA[0]->GetSize(), blocksB[0]->GetSize());
    ASSERT_EQ(blocksA[1]->GetSize(), 4);
    ASSERT_EQ(blocksA[1]->GetSize(), blocksB[1]->GetSize());
    ASSERT_EQ(blocksA[2]->GetSize(), 7);
    ASSERT_EQ(blocksA[2]->GetSize(), blocksB[2]->GetSize());

    GpuSqlCustomParser parserAlter(database, "ALTER TABLE testTable ADD colC int;");
    resultPtr = parserAlter.Parse();

    auto& columnIntC = table.GetColumns().at("colC");
    auto& blocksC = dynamic_cast<ColumnBase<int32_t>*>(columnIntC.get())->GetBlocksList();

    ASSERT_EQ(blocksC[0]->GetSize(), 4);
    ASSERT_EQ(blocksC[0]->GetNullBitmask()[0], 15);
    ASSERT_EQ(blocksC[1]->GetSize(), 4);
    ASSERT_EQ(blocksC[1]->GetNullBitmask()[0], 15);
    ASSERT_EQ(blocksC[2]->GetSize(), 7);
    ASSERT_EQ(blocksC[2]->GetNullBitmask()[0], 127);

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterAddWI;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, AlterTableAddColumn)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE TestDatabaseAlterAdd 15;");
    auto resultPtr = createDatabase.Parse();

    auto database = Database::GetDatabaseByName("TestDatabaseAlterAdd");

    ASSERT_TRUE(database->GetTables().find("testTable") == database->GetTables().end());

    GpuSqlCustomParser parser(database, "CREATE TABLE testTable (colA int, colB int);");
    resultPtr = parser.Parse();

    ASSERT_TRUE(database->GetTables().find("testTable") != database->GetTables().end());

    for (int32_t i = 0; i < 17; i++)
    {
        GpuSqlCustomParser parser2(database, "INSERT INTO testTable (colA, colB) VALUES (" +
                                                 std::to_string(i) + ", " + std::to_string(i % 5) + ");");
        resultPtr = parser2.Parse();
    }

    auto& table = database->GetTables().at("testTable");
    auto& columnIntA = table.GetColumns().at("colA");
    auto& blocksA = dynamic_cast<ColumnBase<int32_t>*>(columnIntA.get())->GetBlocksList();

    auto& columnIntB = table.GetColumns().at("colB");
    auto& blocksB = dynamic_cast<ColumnBase<int32_t>*>(columnIntB.get())->GetBlocksList();

    ASSERT_EQ(blocksA[0]->GetSize(), 15);
    ASSERT_EQ(blocksA[0]->GetSize(), blocksB[0]->GetSize());
    ASSERT_EQ(blocksA[1]->GetSize(), 2);
    ASSERT_EQ(blocksA[1]->GetSize(), blocksB[1]->GetSize());

    GpuSqlCustomParser parserAlter(database, "ALTER TABLE testTable ADD colC int;");
    resultPtr = parserAlter.Parse();

    auto& columnIntC = table.GetColumns().at("colC");
    auto& blocksC = dynamic_cast<ColumnBase<int32_t>*>(columnIntC.get())->GetBlocksList();

    ASSERT_EQ(blocksC[0]->GetSize(), 15);
    ASSERT_EQ(blocksC[0]->GetNullBitmask()[0], -1);
    ASSERT_EQ(blocksC[0]->GetNullBitmask()[1], 127);
    ASSERT_EQ(blocksC[1]->GetSize(), 2);
    ASSERT_EQ(blocksC[1]->GetNullBitmask()[0], 3);
    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE TestDatabaseAlterAdd;");
    resultPtr = parserDropDb.Parse();
}

TEST(DispatcherTests, InsertInto)
{
    GpuSqlCustomParser createDatabase(nullptr, "CREATE DATABASE InsertIntoDb 30;");
    auto resultPtr = createDatabase.Parse();
    auto database = Database::GetDatabaseByName("InsertIntoDb");

    GpuSqlCustomParser parserCreate(database,
                                    "CREATE TABLE testTable (colA int, colB int, Aa int);");
    resultPtr = parserCreate.Parse();
    auto& table = database->GetTables().at("testTable");

    // Insert values into two of three columns
    GpuSqlCustomParser parserInsert(database, "insert into testTable (colA, colB) values (1, 2);");
    resultPtr = parserInsert.Parse();

    // Select right after inserting one row of data - insert into two of three columns
    GpuSqlCustomParser parserSelect(database, "SELECT colA, colB, Aa from testTable;");
    resultPtr = parserSelect.Parse();
    auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    auto payloadsColA = result->payloads().at("testTable.colA");
    auto payloadsColB = result->payloads().at("testTable.colB");
    auto payloadsColAa = result->payloads().at("testTable.Aa");

    ASSERT_EQ(payloadsColA.intpayload().intdata()[0], 1);
    ASSERT_EQ(payloadsColB.intpayload().intdata()[0], 2);
    ASSERT_EQ(payloadsColAa.intpayload().intdata()[0], -2147483648);

    auto& columnIntA = table.GetColumns().at("colA");
    auto& blocksA = dynamic_cast<ColumnBase<int32_t>*>(columnIntA.get())->GetBlocksList();
    auto& columnIntB = table.GetColumns().at("colB");
    auto& blocksB = dynamic_cast<ColumnBase<int32_t>*>(columnIntB.get())->GetBlocksList();
    auto& columnIntAa = table.GetColumns().at("Aa");
    auto& blocksAa = dynamic_cast<ColumnBase<int32_t>*>(columnIntAa.get())->GetBlocksList();

    ASSERT_EQ(blocksA.size(), 1);
    ASSERT_EQ(blocksB.size(), 1);
    ASSERT_EQ(blocksAa.size(), 1);

    ASSERT_EQ(blocksA[0]->GetSize(), 1);
    ASSERT_EQ(blocksB[0]->GetSize(), 1);
    ASSERT_EQ(blocksAa[0]->GetSize(), 1);

    ASSERT_EQ(blocksA[0]->GetNullBitmaskSize(), 1);
    ASSERT_EQ(blocksB[0]->GetNullBitmaskSize(), 1);
    ASSERT_EQ(blocksAa[0]->GetNullBitmaskSize(), 1);

    ASSERT_EQ(blocksA[0]->GetData()[0], 1);
    ASSERT_EQ(blocksB[0]->GetData()[0], 2);
    ASSERT_EQ(blocksAa[0]->GetData()[0], -2147483648);

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[0], 0);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[0], 0);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[0], 1);

    //---------------------------------------------------------
    // Insert 5 more times same values into two of three columns
    for (int32_t i = 0; i < 5; i++)
    {
        resultPtr = parserInsert.Parse();
    }

    GpuSqlCustomParser parserSelect3(database, "SELECT colA, colB, Aa from testTable;");
    resultPtr = parserSelect3.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    payloadsColA = result->payloads().at("testTable.colA");
    payloadsColB = result->payloads().at("testTable.colB");
    payloadsColAa = result->payloads().at("testTable.Aa");

    ASSERT_EQ(blocksA.size(), 1);
    ASSERT_EQ(blocksB.size(), 1);
    ASSERT_EQ(blocksAa.size(), 1);

    ASSERT_EQ(blocksA[0]->GetSize(), 6);
    ASSERT_EQ(blocksB[0]->GetSize(), 6);
    ASSERT_EQ(blocksAa[0]->GetSize(), 6);

    ASSERT_EQ(blocksA[0]->GetNullBitmaskSize(), 1);
    ASSERT_EQ(blocksB[0]->GetNullBitmaskSize(), 1);
    ASSERT_EQ(blocksAa[0]->GetNullBitmaskSize(), 1);

    for (int32_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i], 1);
        ASSERT_EQ(blocksB[0]->GetData()[i], 2);
        ASSERT_EQ(blocksAa[0]->GetData()[i], -2147483648);

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i], 1) << "Iteration: " << i;
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i], 2);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i], -2147483648);
    }

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[0], 0);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[0], 0);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[0], 63);
    
    //---------------------------------------------------------
    // Insert 5 times into third column, which was empty - filled with null values till now
    GpuSqlCustomParser parserInsert2(database, "insert into testTable (Aa) values (3);");

    for (int32_t i = 0; i < 5; i++)
    {
        resultPtr = parserInsert2.Parse();
    }

    GpuSqlCustomParser parserSelect4(database, "SELECT colA, colB, Aa from testTable;");
    resultPtr = parserSelect4.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    payloadsColA = result->payloads().at("testTable.colA");
    payloadsColB = result->payloads().at("testTable.colB");
    payloadsColAa = result->payloads().at("testTable.Aa");

	ASSERT_EQ(blocksA.size(), 1);
    ASSERT_EQ(blocksB.size(), 1);
    ASSERT_EQ(blocksAa.size(), 1);

    ASSERT_EQ(blocksA[0]->GetSize(), 11);
    ASSERT_EQ(blocksB[0]->GetSize(), 11);
    ASSERT_EQ(blocksAa[0]->GetSize(), 11);

    ASSERT_EQ(blocksA[0]->GetNullBitmaskSize(), 2);
    ASSERT_EQ(blocksB[0]->GetNullBitmaskSize(), 2);
    ASSERT_EQ(blocksAa[0]->GetNullBitmaskSize(), 2);

    for (int32_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i], 1);
        ASSERT_EQ(blocksB[0]->GetData()[i], 2);
        ASSERT_EQ(blocksAa[0]->GetData()[i], -2147483648);

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i], 1);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i], 2);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i], -2147483648);
    }

    for (int32_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i + 6], -2147483648);
        ASSERT_EQ(blocksB[0]->GetData()[i + 6], -2147483648);
        ASSERT_EQ(blocksAa[0]->GetData()[i + 6], 3);

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i + 6], 3);
    }

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[0], -64);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[0], -64);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[0], 63);

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[1], 7);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[1], 7);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[1], 0);

    //---------------------------------------------------------
	// Alter table to add one string column - it should be filled with null values
    GpuSqlCustomParser parserAlter(database, "alter table testTable add colString string;");
    resultPtr = parserAlter.Parse();

	GpuSqlCustomParser parserSelect5(database, "SELECT colA, colB, Aa, colString from testTable;");
    resultPtr = parserSelect5.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    payloadsColA = result->payloads().at("testTable.colA");
    payloadsColB = result->payloads().at("testTable.colB");
    payloadsColAa = result->payloads().at("testTable.Aa");
    auto payloadsColString = result->payloads().at("testTable.colString");

    auto& columnString = table.GetColumns().at("colString");
    auto& blocksString = dynamic_cast<ColumnBase<std::string>*>(columnString.get())->GetBlocksList();

	ASSERT_EQ(blocksA.size(), 1);
    ASSERT_EQ(blocksB.size(), 1);
    ASSERT_EQ(blocksAa.size(), 1);
    ASSERT_EQ(blocksString.size(), 1);

    ASSERT_EQ(blocksA[0]->GetSize(), 11);
    ASSERT_EQ(blocksB[0]->GetSize(), 11);
    ASSERT_EQ(blocksAa[0]->GetSize(), 11);
    ASSERT_EQ(blocksString[0]->GetSize(), 11);

    ASSERT_EQ(blocksA[0]->GetNullBitmaskSize(), 2);
    ASSERT_EQ(blocksB[0]->GetNullBitmaskSize(), 2);
    ASSERT_EQ(blocksAa[0]->GetNullBitmaskSize(), 2);
    ASSERT_EQ(blocksString[0]->GetNullBitmaskSize(), 2);

    for (int32_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i], 1);
        ASSERT_EQ(blocksB[0]->GetData()[i], 2);
        ASSERT_EQ(blocksAa[0]->GetData()[i], -2147483648);
        ASSERT_EQ(blocksString[0]->GetData()[i], " ");

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i], 1);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i], 2);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i], -2147483648);
        ASSERT_EQ(payloadsColString.stringpayload().stringdata()[i], " ");
    }

    for (int32_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i + 6], -2147483648);
        ASSERT_EQ(blocksB[0]->GetData()[i + 6], -2147483648);
        ASSERT_EQ(blocksAa[0]->GetData()[i + 6], 3);
        ASSERT_EQ(blocksString[0]->GetData()[i + 6], " ");

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i + 6], 3);
        ASSERT_EQ(payloadsColString.stringpayload().stringdata()[i + 6], " ");
    }

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[0], -64);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[0], -64);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[0], 63);
    ASSERT_EQ(blocksString[0]->GetNullBitmask()[0], -1);

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[1], 7);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[1], 7);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[1], 0);
    ASSERT_EQ(blocksString[0]->GetNullBitmask()[1], 7);

	//---------------------------------------------------------
	// Insert into new column "colString", other columns should be filled with null values  
    GpuSqlCustomParser parserInsert3(database,
                                     "insert into testTable (colString) values (\"abc\");");

    for (int32_t i = 0; i < 3; i++)
    {
        resultPtr = parserInsert3.Parse();
    }

	GpuSqlCustomParser parserSelect6(database, "SELECT colA, colB, Aa, colString from testTable;");
    resultPtr = parserSelect6.Parse();
    result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
    payloadsColA = result->payloads().at("testTable.colA");
    payloadsColB = result->payloads().at("testTable.colB");
    payloadsColAa = result->payloads().at("testTable.Aa");
    payloadsColString = result->payloads().at("testTable.colString");

	for (int32_t i = 0; i < 6; i++)
    {
        ASSERT_EQ(payloadsColA.intpayload().intdata()[i], 1);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i], 2);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i], -2147483648);
        ASSERT_EQ(payloadsColString.stringpayload().stringdata()[i], " ");
    }

    for (int32_t i = 0; i < 5; i++)
    {
        ASSERT_EQ(payloadsColA.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i + 6], -2147483648);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i + 6], 3);
        ASSERT_EQ(payloadsColString.stringpayload().stringdata()[i + 6], " ");
    }

    for (int32_t i = 0; i < 3; i++)
    {
        ASSERT_EQ(blocksA[0]->GetData()[i + 11], -2147483648);
        ASSERT_EQ(blocksB[0]->GetData()[i + 11], -2147483648);
        ASSERT_EQ(blocksAa[0]->GetData()[i + 11], -2147483648);
        ASSERT_EQ(blocksString[0]->GetData()[i + 11], "abc");

		ASSERT_EQ(payloadsColA.intpayload().intdata()[i + 11], -2147483648);
        ASSERT_EQ(payloadsColB.intpayload().intdata()[i + 11], -2147483648);
        ASSERT_EQ(payloadsColAa.intpayload().intdata()[i + 11], -2147483648);
        ASSERT_EQ(payloadsColString.stringpayload().stringdata()[i + 11], "abc");
    }

    ASSERT_EQ(blocksA[0]->GetNullBitmask()[1], 63);
    ASSERT_EQ(blocksB[0]->GetNullBitmask()[1], 63);
    ASSERT_EQ(blocksAa[0]->GetNullBitmask()[1], 56);
    ASSERT_EQ(blocksString[0]->GetNullBitmask()[1], 7);

    GpuSqlCustomParser parserDropDb(database, "DROP DATABASE InsertIntoDb;");
    resultPtr = parserDropDb.Parse();
}