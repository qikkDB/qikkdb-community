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

//INT ">"
TEST(DispatcherTests, IntGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 500 > colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger2 FROM TableA WHERE colInteger2 > colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger2");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 > 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG ">"
TEST(DispatcherTests, LongGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 > 500000000;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 > colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong2 FROM TableA WHERE colLong2 > colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) > (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
				}
			}
			else
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 > (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
				}
			}

		}
	}

	auto &payloads = result->payloads().at("TableA.colLong2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 > 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT ">"
TEST(DispatcherTests, FloatGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 > 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.5 > colFloat1;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat2 FROM TableA WHERE colFloat2 > colFloat1;");
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

	auto &payloads = result->payloads().at("TableA.colFloat2");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10 > 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE ">"
TEST(DispatcherTests, DoubleGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 > 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.5 > colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble2 FROM TableA WHERE colDouble2 > colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble2");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10 > 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "<" operator
/////////////////////

//INT "<"
TEST(DispatcherTests, IntLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 < 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 500 < colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 < colInteger2;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 < 10;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 < 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "<"
TEST(DispatcherTests, LongLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 < 500000000;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 < colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 < colLong2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) > (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
				}
			}
			else
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 > (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 < 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 < 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT "<"
TEST(DispatcherTests, FloatLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 < 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.5 < colFloat1;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 < colFloat2;");
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
				if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) < (static_cast<float>((j % 2048 + 0.1111) * (-1))))
				{
					expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 < 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10 < 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE "<"
TEST(DispatcherTests, DoubleLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 < 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.5 < colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 < colDouble2;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 < 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10 < 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   ">=" operator
/////////////////////

//INT ">="
TEST(DispatcherTests, IntEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 >= 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 500 >= colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger2 FROM TableA WHERE colInteger2 >= colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger2");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 >= 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 >= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG ">="
TEST(DispatcherTests, LongEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 >= 500000000;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 >= colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong2 FROM TableA WHERE colLong2 >= colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
				}
			}
			else
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 >= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 >= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT ">="
TEST(DispatcherTests, FloatEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 >= 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.5 >= colFloat1;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat2 FROM TableA WHERE colFloat2 >= colFloat1;");
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
				if ((static_cast<float>((j % 2048 + 0.1111) * (-1))) >= (static_cast<float>((j % 1024 + 0.1111) * (-1))))
				{
					expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat2");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10 >= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 >= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE ">="
TEST(DispatcherTests, DoubleEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 >= 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqGtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.5 >= colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqGtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble2 FROM TableA WHERE colDouble2 >= colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble2");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqGtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10 >= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleEqGtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 >= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "<=" operator
/////////////////////

//INT "<="
TEST(DispatcherTests, IntEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 <= 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 500 <= colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 <= colInteger2;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 <= 10;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 <= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "<="
TEST(DispatcherTests, LongEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 <= 500000000;");
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


	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 <= colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 <= colLong2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) >= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
				}
			}
			else
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 >= (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 <= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 <= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT "<="
TEST(DispatcherTests, FloatEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 <= 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.5 <= colFloat1;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 <= colFloat2;");
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
				if ((static_cast<float>((j % 1024 + 0.1111) * (-1))) <= (static_cast<float>((j % 2048 + 0.1111) * (-1))))
				{
					expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 <= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10 <= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE "<="
TEST(DispatcherTests, DoubleEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 <= 5.5;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqLtConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.5 <= colDouble1;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqLtColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 <= colDouble2;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqLtConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 <= 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleEqLtConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10 <= 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "=" operator
/////////////////////

//INT "="
TEST(DispatcherTests, IntEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 = 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if ((j % 1024) == 5)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 = colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (5 == j % 1024)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger2 FROM TableA WHERE colInteger2 = colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 2048) == (j % 1024))
			{
				(j % 2) ? expectedResult.push_back(j % 2048) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger2");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 = 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 = 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "="
TEST(DispatcherTests, LongEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 = 500000000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 = colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong2 FROM TableA WHERE colLong2 = colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) == (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
				}
			}
			else
			{
				if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1) == (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 = 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 = 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT "="
TEST(DispatcherTests, FloatEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 = 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}

}

TEST(DispatcherTests, FloatEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 = colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat2 FROM TableA WHERE colFloat2 = colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs((static_cast<float>(j % 2048 + 0.1111)) - (static_cast<float>(j % 1024 + 0.1111))) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat2");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 = 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 = 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE "="
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble2 FROM TableA WHERE colDouble2 = colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs((j % 2048 + 0.1111111) - (j % 1024 + 0.1111111)) < 0.00000005)
			{
				(j % 2) ? expectedResult.push_back(j % 2048 + 0.1111111) : expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble2");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 = 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 = 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   "!=" operator
/////////////////////

//INT "!="
TEST(DispatcherTests, IntNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 != 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if ((j % 1024) != 5)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntNotEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 500 != colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntNotEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger2 FROM TableA WHERE colInteger2 != colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger2");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntNotEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 != 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntNotEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 != 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG "!="
TEST(DispatcherTests, LongNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 != 50000000;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongNotEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE -500000000 != colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongNotEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong2 FROM TableA WHERE colLong2 != colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) != (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048);
				}
			}
			else
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 != (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1)
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongNotEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 != 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongNotEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 != 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT "!="
TEST(DispatcherTests, FloatNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 != 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatNotEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 != colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs((static_cast<float>(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatNotEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat2 FROM TableA WHERE colFloat2 != colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs((static_cast<float>(j % 2048 + 0.1111))-(static_cast<float>(j % 1024 + 0.1111))) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 2048 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 2048 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat2");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatNotEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 != 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatNotEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5 != 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE "!="
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble2 FROM TableA WHERE colDouble2 != colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs((j % 2048 + 0.1111111) - (j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(j % 2048 + 0.1111111) : expectedResult.push_back((j % 2048 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble2");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleNotEqConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 != 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleNotEqConstConstFalse)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5 != 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

/////////////////////
//   AND
/////////////////////

//INT AND
TEST(DispatcherTests, IntAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 AND 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if ((j % 1024) != 0)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAndColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 AND colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if ((j % 1024) != 0)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAndConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 AND colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 AND colInteger2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 2048 != 0) && (j % 1024 != 0))
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAndConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 AND 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAndConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 AND 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG AND
TEST(DispatcherTests, LongAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 AND 500000000;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAndColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}
TEST(DispatcherTests, LongAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 AND colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAndConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 AND colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 AND colLong2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048 != 0) && (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
				}
			}
			else
			{
				if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 != 0) && ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0))
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAndConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 AND 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAndConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 AND 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT AND
TEST(DispatcherTests, FloatAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 AND 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatAndColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 AND colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatAndConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 AND colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat2 AND colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) && (static_cast<float>(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j %1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}
TEST(DispatcherTests, FloatAndConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10.1111 AND 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));

		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatAndConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 AND 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE AND
TEST(DispatcherTests, DoubleAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 AND 5.1111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) : expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleAndColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.1111111 AND colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleAndConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 AND colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble2 AND colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 2048 + 0.1111111 > std::numeric_limits<double>::epsilon()) && (j % 1024 + 0.1111111 > std::numeric_limits<double>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}
TEST(DispatcherTests, DoubleAndConstConstTrue)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10.1111111 AND 5.1111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleAndConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.11111111 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 AND 5.11111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}


/////////////////////
//   OR
/////////////////////

//INT OR
TEST(DispatcherTests, IntOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 OR 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 1024) != 0)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 OR colInteger1;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 OR colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 1024) != 0)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 OR colInteger2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 2048 != 0) || (j % 1024 != 0))
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstConstNonZeroValues)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 OR 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 10 OR 0;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 OR 5;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntOrConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

// LONG OR
TEST(DispatcherTests, LongOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 OR 500000000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 OR 0;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 OR colLong1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 OR colLong1;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 OR colLong2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048 != 0) || (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 != 0))
				{
					expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024);
				}
			}
			else
			{
				if (((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 2048) * -1 != 0) || ((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 != 0))
				{
					expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
				}
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstConstNonZeroValues)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 10 OR 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 5 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 OR 5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
		}
	}

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongOrConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//FLOAT OR
TEST(DispatcherTests, FloatOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 OR 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 OR colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 OR colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat2 OR colFloat1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((static_cast<float>(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) || (static_cast<float>(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}
TEST(DispatcherTests, FloatOrConstConstNonZeroValues)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 10.1111 OR 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 OR 5.1111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<float>(j % 1024 + 0.1111)) : expectedResult.push_back(static_cast<float>((j % 1024 + 0.1111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatOrConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

//DOUBLE OR
TEST(DispatcherTests, DoubleOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 OR 5.1111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) : expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleOrColumnConstZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((j % 1024 + 0.1111111)) : expectedResult.push_back(((j % 1024 + 0.1111111) * (-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleOrConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.1111111 OR colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleOrConstColumnZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 OR colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((j % 1024 + 0.1111111)) > std::numeric_limits<double>::epsilon())
			{
				(j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleOrColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble2 OR colDouble1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 2048 + 0.1111111 > std::numeric_limits<double>::epsilon()) || (j % 1024 + 0.1111111 > std::numeric_limits<double>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back(j % 1024 + 0.1111111) : expectedResult.push_back((j % 1024 + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleOrConstConstNonZeroValues)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 10.1111111 OR 5.1111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleOrConstConstFalseRightZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.11111111 OR 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleOrConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 OR 5.11111111;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
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
}

TEST(DispatcherTests, DoubleOrConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 OR 0;");
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE !(colInteger1 > 5);");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE !(colLong1 > 500000000);");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE !(colFloat1 > 6.5555);");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE !(colDouble1 > 9.66666666);");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 + 5 FROM TableA;");
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

	auto &payloads = result->payloads().at("colInteger1+5");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAddColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 + 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntAddColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 + 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongAddColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 + 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) + 5) : expectedResult.push_back((((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)) * -1) + 5);
		}
	}

	auto &payloads = result->payloads().at("colLong1+5");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAddColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 + 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongAddColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 + 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, FloatAddColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 + 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) + 5.1111) : expectedResult.push_back(((j % 1024) + 0.1111) * (-1) + 5);
		}
	}

	auto &payloads = result->payloads().at("colFloat1+5");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatAddColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 + 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatAddColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 + 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, DoubleAddColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 + 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) + 5.1111111) : expectedResult.push_back(((j % 1024) + 0.1111111) * (-1) + 5);
		}
	}

	auto &payloads = result->payloads().at("colDouble1+5");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleAddColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 + 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleAddColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 + 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, IntSubColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 - 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) - 5) : expectedResult.push_back(((j % 1024) * -1) - 5);
		}
	}

	auto &payloads = result->payloads().at("colInteger1-5");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntSubColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 - 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntSubColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 - 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongSubColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 - 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) - 5) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1 - 5);
		}
	}

	auto &payloads = result->payloads().at("colLong1-5");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongSubColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 - 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongSubColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 - 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, FloatSubColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 - 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(j % 1024 + 0.1111 - 5) : expectedResult.push_back(((j % 1024 + 0.1111) * -1) - 5);
		}
	}

	auto &payloads = result->payloads().at("colFloat1-5");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_TRUE(std::abs(expectedResult[i] - payloads.floatpayload().floatdata()[i]) < 0.0005);
	}
}

TEST(DispatcherTests, FloatSubColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 - 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatSubColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 - 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, DoubleSubColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 - 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) + 0.1111111 - 5) : expectedResult.push_back((((j % 1024) + 0.1111111) * (-1)) - 5);
		}
	}

	auto &payloads = result->payloads().at("colDouble1-5");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleSubColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 - 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleSubColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 - 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

//multiply tests:
TEST(DispatcherTests, IntMulColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 * 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) * 5) : expectedResult.push_back(((j % 1024) * 5) * ((-1)));
		}
	}

	auto &payloads = result->payloads().at("colInteger1*5");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntMulColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 * 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntMulColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 * 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongMulColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 * 2 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * 2) : expectedResult.push_back(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) * 2);
		}
	}

	auto &payloads = result->payloads().at("colLong1*2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongMulColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 * 2 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongMulColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 * 2 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, FloatMulColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 * 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111) * 5) : expectedResult.push_back((((j % 1024) + 0.1111) * 5) * ((-1)));
		}
	}

	auto &payloads = result->payloads().at("colFloat1*5");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_TRUE(std::abs(expectedResult[i] - payloads.floatpayload().floatdata()[i]) < 0.0005);
	}
}

TEST(DispatcherTests, FloatMulColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 * 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatMulColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 * 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, DoubleMulColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 * 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111111) * 5) : expectedResult.push_back((((j % 1024) + 0.1111111) * 5) * ((-1)));
		}
	}

	auto &payloads = result->payloads().at("colDouble1*5");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleMulColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 * 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleMulColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 * 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

//divide tests:
TEST(DispatcherTests, IntDivColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 / 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int32_t>((j % 1024) / 5)) : expectedResult.push_back(static_cast<int32_t>(((j % 1024) / 5) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("colInteger1/5");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntDivColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 / 2 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 2)) : expectedResult.push_back(static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 2));
		}
	}

	auto &payloads = result->payloads().at("colLong1/2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 / 5 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 / 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, FloatDivColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 / 5 FROM TableA;");
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

	auto &payloads = result->payloads().at("colFloat1/5");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloatDivColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 / 5 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);


}

TEST(DispatcherTests, FloatDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 / 5 < 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((((j % 1024) + 0.1111) / 5) < 500)
			{
				(j % 2) ? expectedResult.push_back((j % 1024) + 0.1111) : expectedResult.push_back(((j % 1024) + 0.1111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, DoubleDivColumnConst) //FIXME Dispatch je chybny, treba ho opravit, test je dobry
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 / 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(((j % 1024) + 0.1111111) / 5) : expectedResult.push_back((((j % 1024) + 0.1111111) * -1) / 5);
		}
	}

	auto &payloads = result->payloads().at("colDouble1/5");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, DoubleDivColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 / 5 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, DoubleDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 / 5 < 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((((j % 1024) + 0.1111111) / 5) < 500)
			{
				(j % 2) ? expectedResult.push_back((j % 1024) + 0.1111111) : expectedResult.push_back(((j % 1024) + 0.1111111) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), expectedResult.size());

	for (int i = 0; i < payloads.doublepayload().doubledata_size(); i++)
	{
		ASSERT_DOUBLE_EQ(expectedResult[i], payloads.doublepayload().doubledata()[i]);
	}
}

TEST(DispatcherTests, IntDivColumnConstFloat) //FIXME chyba je v CUDA kerneli, ma vracat float
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 / 5.0 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((j % 1024) / 5.0) : expectedResult.push_back(((j % 1024) * -1) / 5.0);
		}
	}

	auto &payloads = result->payloads().at("colInteger1/5.0");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, IntDivColumnConstGtConstFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5.0 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntDivColumnConstLtConstFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5.0 < 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 1024) / 5.0 < 500)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConstFloat) //FIXME test je dobry, kernel treba spravi tak aby to pretypoval na double
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 / 2.0 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) / 2.0) : expectedResult.push_back(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1) / 2.0);
		}
	}

	auto &payloads = result->payloads().at("colLong1/2.0");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConstGtConstFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 / 5.0 > 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongDivColumnConstLtConstFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 / 5.0 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

//modulo tests:
//divide tests:
TEST(DispatcherTests, IntModColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 % 5 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int32_t>((j % 1024) % 5)) : expectedResult.push_back(static_cast<int32_t>(((j % 1024) % 5) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("colInteger1%5");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, IntModColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 % 5 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, IntModColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 % 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LongModColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 % 2 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back(static_cast<int64_t>((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) % 2)) : expectedResult.push_back(static_cast<int64_t>((static_cast<int64_t>((2 * pow(10, j % 19)) + j % 1024) % 2) * (-1)));
		}
	}

	auto &payloads = result->payloads().at("colLong1%2");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, LongModColumnConstGtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 % 5 > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, LongModColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 % 5 < 500;");
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

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), expectedResult.size());

	for (int i = 0; i < payloads.int64payload().int64data_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, DateTimeNow)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT YEAR(NOW()), MONTH(NOW()), DAY(NOW()), HOUR(NOW()), MINUTE(NOW()) FROM TableA;");
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

	auto &payloadsYear = result->payloads().at("YEAR(NOW())");
	auto &payloadsMonth = result->payloads().at("MONTH(NOW())");
	auto &payloadsDay = result->payloads().at("DAY(NOW())");
	auto &payloadsHour = result->payloads().at("HOUR(NOW())");
	auto &payloadsMinute = result->payloads().at("MINUTE(NOW())");

	ASSERT_EQ(payloadsYear.intpayload().intdata_size(), expectedResultsYear.size());
	ASSERT_EQ(payloadsMonth.intpayload().intdata_size(), expectedResultsMonth.size());
	ASSERT_EQ(payloadsDay.intpayload().intdata_size(), expectedResultsDay.size());
	ASSERT_EQ(payloadsHour.intpayload().intdata_size(), expectedResultsHour.size());
	ASSERT_EQ(payloadsMinute.intpayload().intdata_size(), expectedResultsMinute.size());

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
}


// DateTime tests
TEST(DispatcherTests, DateTimeCol)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT YEAR(colLong3), MONTH(colLong3), DAY(colLong3), HOUR(colLong3), MINUTE(colLong3), SECOND(colLong3) FROM TableA;");
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
		for (int k = 0; k < (1<<11); k++)
		{
			expectedResultsYear.push_back(static_cast<int32_t>(k % 1000) + 2000);
			expectedResultsMonth.push_back(static_cast<int32_t>((k % 12) + 1));
			expectedResultsDay.push_back(static_cast<int32_t>(((k % 28) + 1)));
			expectedResultsHour.push_back(static_cast<int32_t>((k % 24)));
			expectedResultsMinute.push_back(static_cast<int32_t>(((k + 1) % 60)));
			expectedResultsSecond.push_back(static_cast<int32_t>((k + 2) % 60));
		}
	}

	auto &payloadsYear = result->payloads().at("YEAR(colLong3)");
	auto &payloadsMonth = result->payloads().at("MONTH(colLong3)");
	auto &payloadsDay = result->payloads().at("DAY(colLong3)");
	auto &payloadsHour = result->payloads().at("HOUR(colLong3)");
	auto &payloadsMinute = result->payloads().at("MINUTE(colLong3)");
	auto &payloadsSecond = result->payloads().at("SECOND(colLong3)");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPolygon1 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPolygons;

	auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colPolygon1").get());

	for (int i = 0; i < 2; i++)
	{
		auto block = column->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsPolygons.push_back(ComplexPolygonFactory::WktFromPolygon(block->GetData()[k], true));
		}
	}

	auto &payloads = result->payloads().at("TableA.colPolygon1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPolygons.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPolygons[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, RetPolygonsWhere)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPolygon1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPolygons;

	auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::ComplexPolygon>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colPolygon1").get());

	for (int i = 0; i < 2; i++)
	{
		auto block = column->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			if ((k % 1024) * (k % 2 ? 1 : -1) < 20)
			{
				expectedResultsPolygons.push_back(ComplexPolygonFactory::WktFromPolygon(block->GetData()[k], true));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colPolygon1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPolygons.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPolygons[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, RetPoints)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPoint1 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colPoint1").get());

	for (int i = 0; i < 2; i++)
	{
		auto block = column->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k], true));
		}
	}

	auto &payloads = result->payloads().at("TableA.colPoint1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, RetPointsWhere)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPoint1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colPoint1").get());

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

	auto &payloads = result->payloads().at("TableA.colPoint1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, RetString)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colString1 FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsStrings;

	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colString1").get());

	for (int i = 0; i < 2; i++)
	{
		auto block = column->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsStrings.push_back(block->GetData()[k]);
		}
	}

	auto &payloads = result->payloads().at("TableA.colString1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsStrings.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsStrings[i], payloads.stringpayload().stringdata()[i]) << " at row " << i;
	}
}

TEST(DispatcherTests, RetStringWhere)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colString1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsStrings;

	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colString1").get());

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

	auto &payloads = result->payloads().at("TableA.colString1");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsStrings.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsStrings[i], payloads.stringpayload().stringdata()[i]) << " at row " << i;
	}
}

TEST(DispatcherTests, PointFromColCol)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(colInteger1, colFloat1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		auto blockFloat = columnFloat->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			std::ostringstream wktStream;
			wktStream << std::fixed;
			wktStream << std::setprecision(4);
			wktStream << "POINT(" << static_cast<float>(blockInt->GetData()[k]) << " " << blockFloat->GetData()[k] << ")";
			expectedResultsPoints.push_back(wktStream.str());
		}
	}

	auto &payloads = result->payloads().at("POINT(colInteger1,colFloat1)");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, PointFromColConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(colInteger1, 4.5) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			std::ostringstream wktStream;
			wktStream << std::fixed;
			wktStream << std::setprecision(4);
			wktStream << "POINT(" << static_cast<float>(blockInt->GetData()[k]) << " 4.5000" << ")";
			expectedResultsPoints.push_back(wktStream.str());
		}
	}

	auto &payloads = result->payloads().at("POINT(colInteger1,4.5)");

	ASSERT_EQ(payloads.stringpayload().stringdata_size(), expectedResultsPoints.size());

	for (int i = 0; i < payloads.stringpayload().stringdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsPoints[i], payloads.stringpayload().stringdata()[i]);
	}
}

TEST(DispatcherTests, PointFromConstCol)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(7, colFloat1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockFloat = columnFloat->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			std::ostringstream wktStream;
			wktStream << std::fixed;
			wktStream << std::setprecision(4);
			wktStream << "POINT(" << "7.0000 " << blockFloat->GetData()[k] << ")";
			expectedResultsPoints.push_back(wktStream.str());
		}
	}

	auto &payloads = result->payloads().at("POINT(7,colFloat1)");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT MIN(colInteger1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("MIN(colInteger1)");

	// Get the input column
	const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
		reinterpret_cast<ColumnBase<int32_t>*>(
			DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get())
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT MAX(colInteger1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("MAX(colInteger1)");

	// Get the input column
	const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
		reinterpret_cast<ColumnBase<int32_t>*>(
			DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get())
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT SUM(colInteger1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("SUM(colInteger1)");

	// Get the input column
	const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
		reinterpret_cast<ColumnBase<int32_t>*>(
			DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get())
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT AVG(colInteger1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("AVG(colInteger1)");

	// Get the input column
	const std::vector<BlockBase<int32_t>*>& inputColumn1Blocks =
		reinterpret_cast<ColumnBase<int32_t>*>(
			DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get())
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT COUNT(colInteger1) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("COUNT(colInteger1)");
	ASSERT_EQ(payloads.int64payload().int64data_size(), 1);
	ASSERT_EQ(payloads.int64payload().int64data()[0], TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
}


TEST(DispatcherTests, Alias)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT (t.colInteger1 - 10) AS col1, t.colFloat1 AS col2 FROM TableA as t WHERE t.colInteger1 > 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;
	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsInt = result->payloads().at("col1");
	auto &payloadsFloat = result->payloads().at("col2");

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10 OFFSET 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, Limit)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, Offset)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 OFFSET 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), trimmedExpectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(trimmedExpectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LargeOffset)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 OFFSET 10000000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());


	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LargeLimit)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10000000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseOrColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 | 20) > 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseAndColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 & 20) > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseXorColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ 20) > 100;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseLeftShiftColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 << 2) > 100;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseRightShiftColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 >> 2) > 100;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseOrColColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 | colInteger2) > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger2").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseAndColColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 & colInteger2) > 10;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger2").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, BitwiseXorColColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ colInteger2) > 500;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnInt2 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger2").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, NotEqualsAlternativeOperator)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 <> 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, MinusColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE -colInteger1 = 3;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), 0);
}

TEST(DispatcherTests, AbsColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ABS(colInteger1) = 3;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SinColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colInteger1) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, CosColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE COS(colInteger1) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, TanColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE TAN(colInteger1) > 2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SinColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, CosColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE COS(colFloat1) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, TanColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE TAN(colFloat1) > 2;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SinPiColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1 + PI()) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInteger = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, ArcSinColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ASIN(colInteger1 / 1024.0) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			if (asin(blockInt->GetData()[k]/1024.0f) > 0.5)
			{
				expectedResultsInt.push_back(blockInt->GetData()[k]);
			}
		}
	}

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, ArcCosColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ACOS(colInteger1 / 1024.0) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, ArcTanColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ATAN(colInteger1) > 0.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, Logarithm10ColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG10(colInteger1) > 1.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LogarithmNaturalColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1) > 1.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, LogarithmColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1, 3.0) > 1.5;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}
TEST(DispatcherTests, ExponentialColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE EXP(colInteger1) > 2000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, PowerColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE POW(colInteger1, 2) > 2000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SqrtColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SQRT(colInteger1) > 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SquareColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SQUARE(colInteger1) > 2000;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SignPositiveColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SignNegativeColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = -1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, SignZeroColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, RootColConstInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ROOT(colInteger1, 2) > 0;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}


TEST(DispatcherTests, RoundColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ROUND(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("ROUND(colFloat1)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, FloorColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT FLOOR(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("FLOOR(colFloat1)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, CeilColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT CEIL(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("CEIL(colFloat1)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, RoundColInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ROUND(colInteger1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsFloat = result->payloads().at("ROUND(colInteger1)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, CotColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT COT(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("COT(colFloat1)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, Atan2ColFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ATAN2(colFloat1, colFloat1 + 1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("ATAN2(colFloat1,colFloat1+1)");

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
ColmnarDB::NetworkClient::Message::QueryResponsePayload RunFunctionQuery(
	std::string function, std::string column, std::string table)
{
	std::string retFunCol = function + "(" + column + ")";
	return RunQuery(retFunCol, "FROM " + table);
}

/// Run query SELECT function(column) FROM table; and return result payload
ColmnarDB::NetworkClient::Message::QueryResponsePayload RunFunctionColConstQuery(
	std::string function, std::string column, std::string cnst, std::string table)
{
	std::string retFunCol = function + "(" + column +"," + cnst + ")";
	return RunQuery(retFunCol, "FROM " + table);
}

TEST(DispatcherTests, StringLower)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto payloads = RunFunctionQuery("LOWER", col, table);

	std::vector<std::string> expectedResultsStrings;
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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

	std::string edited(text.substr(1, text.length() - 2));;
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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());
	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colIntName).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colIntName).get());

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
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());
	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colIntName).get());

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
	auto column = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(col).get());

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
	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colIntName).get());

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
	auto payloads = RunQuery("CONCAT("+ table + "." + colStrName + "," + table + "." + colStrName + ")", "FROM " + table);
	
	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + colStrName + " = \"" + text + "\"");

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + colStrName + " != \"" + text + "\"");

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + "\"" + text + "\"" + " = " + colStrName);

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + "\"" + text + "\"" + " != " + colStrName);

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + "\"" + text + "\"" + " = \"" + text + "\"");

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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
	auto payloads = RunQuery(table + "." + colStrName, "FROM " + table + " WHERE " + "\"" + text + "diff" + "\"" + " != \"" + text + "\"");

	std::vector<std::string> expectedResultsStrings;
	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->
		GetTables().at(table).GetColumns().at(colStrName).get());

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "CREATE DATABASE [createdDb%^&*()-+];");
	auto resultPtr = parser.Parse();

	ASSERT_TRUE(Database::Exists("createdDb%^&*()-+"));

	GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database, "DROP DATABASE [createdDb%^&*()-+];");
	resultPtr = parser2.Parse();

	ASSERT_TRUE(!Database::Exists("createdDb%^&*()-+"));
}

TEST(DispatcherTests, CreateAlterDropTable)
{
	Context::getInstance();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") == DispatcherObjs::GetInstance().database->GetTables().end());

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "CREATE TABLE tblA (colA int, colB float, INDEX ind (colA, colB));");
	auto resultPtr = parser.Parse();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") != DispatcherObjs::GetInstance().database->GetTables().end());

	std::vector<std::string> expectedSortingColumns = { "colA", "colB" };
	std::vector<std::string> resultSortingColumns = DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetSortingColumns();

	ASSERT_TRUE(expectedSortingColumns.size() == resultSortingColumns.size());

	for (int i = 0; i < expectedSortingColumns.size(); i++)
	{
		ASSERT_TRUE(expectedSortingColumns[i] == resultSortingColumns[i]);
	}

	GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database, "INSERT INTO tblA (colA, colB) VALUES (1, 2.0);");

	for (int32_t i = 0; i < 5; i++)
	{
		resultPtr = parser2.Parse();
	}

	GpuSqlCustomParser parser3(DispatcherObjs::GetInstance().database, "SELECT colA, colB from tblA;");
	resultPtr = parser3.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsColA;
	std::vector<float> expectedResultsColB;

	for (int k = 0; k < 5; k++)
	{
		expectedResultsColA.push_back(1);
		expectedResultsColB.push_back(2.0);
	}

	auto &payloadsColA = result->payloads().at("tblA.colA");
	auto &payloadsColB = result->payloads().at("tblA.colB");

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

	GpuSqlCustomParser parser4(DispatcherObjs::GetInstance().database, "ALTER TABLE tblA DROP COLUMN colA, ADD colC float;");
	resultPtr = parser4.Parse();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetColumns().find("colA")
		== DispatcherObjs::GetInstance().database->GetTables().at("tblA").GetColumns().end());


	GpuSqlCustomParser parser5(DispatcherObjs::GetInstance().database, "SELECT colB, colC from tblA;");
	resultPtr = parser5.Parse();
	result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloadsColB2 = result->payloads().at("tblA.colB");
	auto &payloadsColC = result->payloads().at("tblA.colC");


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

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA") == DispatcherObjs::GetInstance().database->GetTables().end());
}

TEST(DispatcherTests, IsNull)
{
	Context::getInstance();
	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colInteger1 IS NULL;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	ASSERT_EQ(result->payloads().size(),0);
}

TEST(DispatcherTests, IsNotNull)
{
	Context::getInstance();
	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colInteger1 IS NOT NULL;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	std::vector<float> expectedResultsFloat;
	auto column = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->
		GetTables().at("TableA").GetColumns().at("colFloat1").get());
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

//TEST(DispatcherTests, WhereEvaluation)
//{
//	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colInteger1 FROM TableA WHERE ((colInteger2 != 500) AND (colInteger2 > 1000000)) OR ((colInteger1 >= 150) AND (colInteger1 < -1000000));");
//	auto resultPtr = parser.Parse();
//	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
//
//	FAIL();
//}
//
//TEST(DispatcherTests, WhereEvaluationColColPropagation)
//{
//	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database_, "SELECT colInteger1 FROM TableA WHERE ((colInteger2 > colInteger1) AND (colInteger2 > 1000000)) OR ((colInteger1 >= 150) AND (colInteger1 < -1000000));");
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

	GpuSqlCustomParser parserCreateTable(Database::GetDatabaseByName("WhereEvalDatabase"), "CREATE TABLE TableA (ColA INT, ColB INT, ColC INT, ColD INT, INDEX IndA(ColA, ColB, ColC));");
	resultPtr = parserCreateTable.Parse();

	std::vector<int32_t> dataIntA({ 1,  2, 3, 4,  5, 12, 17, 16, 19, 20, 1, 5, 3, 4,  2, 40, 150, 59, 110, 70});
	std::vector<int32_t> dataIntB({ 4, 10, 1, 2,  3,  1,  3,  2,  3,  2, 7, 1, 1, 2, 10,  1,   1,  1,   1,  1});
	std::vector<int32_t> dataIntC({ 6,  8, 1, 2,  1,  1,  3,  1,  4,  1, 7, 3, 2, 1,  6,  1,   1,  1,   1,  1});
	std::vector<int32_t> dataIntD({ 1,  4, 5, 8, 10, 20, 40, 30, 50,  1, 2, 9, 6, 7,  3,  2,   6,  3,   5,  4});

	for (int32_t i = 0; i < dataIntA.size(); i++)
	{
		GpuSqlCustomParser parserInsertInto(Database::GetDatabaseByName("WhereEvalDatabase"), "INSERT INTO TableA (ColA, ColB, ColC, ColD) VALUES (" + GetInsertIntoValuesString({ dataIntA, dataIntB, dataIntC, dataIntD }, i) + ");");
		resultPtr = parserInsertInto.Parse();
	}

	std::vector<int32_t> dataIntASorted({ 1, 1,  2,  2, 3, 3, 4, 4, 5,  5, 12, 16, 17, 19, 20, 40, 59, 70, 110, 150});
	std::vector<int32_t> dataIntBSorted({ 4, 7, 10, 10, 1, 1, 2, 2, 1,  3,  1,  2,  3,  3,  2,  1,  1,  1,   1,   1});
	std::vector<int32_t> dataIntCSorted({ 6, 7,  6,  8, 1, 2, 1, 2, 3,  1,  1,  1,  3,  4,  1,  1,  1,  1,   1,   1});
	std::vector<int32_t> dataIntDSorted({ 1, 2,  3,  4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50,  1,  2,  3,  4,   5,   6});

	std::shared_ptr<Database> database = Database::GetDatabaseByName("WhereEvalDatabase");
	auto& table = database->GetTables().at("TableA");

	//ColA testing for correct sort
	auto& blocksColA = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColA").get())->GetBlocksList();
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

	//ColB testing for correct sort
	auto& blocksColB = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColB").get())->GetBlocksList();
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

	//ColC testing for correct sort
	auto& blocksColC = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColC").get())->GetBlocksList();
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

	//ColD testing for correct sort
	auto& blocksColD = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColD").get())->GetBlocksList();
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

	GpuSqlCustomParser parser(Database::GetDatabaseByName("WhereEvalDatabase"), "SELECT ColA FROM TableA WHERE (ColA >= 10 AND ColA < 50) AND ((5 < (ColB + ColC)) AND SIN(ColD));");
	resultPtr = parser.Parse();
	LoadColHelper& loadColHelper = LoadColHelper::getInstance();

	ASSERT_EQ(loadColHelper.countSkippedBlocks, 2);

	GpuSqlCustomParser parserDropDatabase(nullptr, "DROP DATABASE WhereEvalDatabase;");
	resultPtr = parserDropDatabase.Parse();
}

TEST(DispatcherTests, WhereEvaluationAdvanced_FourTimesAnd)
{
	GpuSqlCustomParser parserCreateDatabase(nullptr, "CREATE DATABASE WhereEvalDatabase 10;");
	auto resultPtr = parserCreateDatabase.parse();

	GpuSqlCustomParser parserCreateTable(Database::GetDatabaseByName("WhereEvalDatabase"), "CREATE TABLE TableA (ColA INT, ColB INT, ColC INT, ColD INT, INDEX IndA(ColA, ColB, ColC));");
	resultPtr = parserCreateTable.parse();

	std::vector<int32_t> dataIntA({ 1,  2, 3, 4,  5, 12, 17, 16, 19, 20, 1, 5, 3, 4,  2, 40, 150, 59, 110, 70});
	std::vector<int32_t> dataIntB({ 4, 10, 1, 2,  3,  1,  3,  2,  3,  2, 7, 1, 1, 2, 10,  1,   1,  1,   1,  1});
	std::vector<int32_t> dataIntC({ 6,  8, 1, 2,  1,  1,  3,  1,  4,  1, 7, 3, 2, 1,  6,  1,   1,  1,   1,  1});
	std::vector<int32_t> dataIntD({ 1,  4, 5, 8, 10, 20, 40, 30, 50,  1, 2, 9, 6, 7,  3,  2,   6,  3,   5,  4});

	for (int32_t i = 0; i < dataIntA.size(); i++)
	{
		GpuSqlCustomParser parserInsertInto(Database::GetDatabaseByName("WhereEvalDatabase"), "INSERT INTO TableA (ColA, ColB, ColC, ColD) VALUES (" + GetInsertIntoValuesString({ dataIntA, dataIntB, dataIntC, dataIntD }, i) + ");");
		resultPtr = parserInsertInto.parse();
	}

	std::vector<int32_t> dataIntASorted({ 1, 1,  2,  2, 3, 3, 4, 4, 5,  5, 12, 16, 17, 19, 20, 40, 59, 70, 110, 150});
	std::vector<int32_t> dataIntBSorted({ 4, 7, 10, 10, 1, 1, 2, 2, 1,  3,  1,  2,  3,  3,  2,  1,  1,  1,   1,   1});
	std::vector<int32_t> dataIntCSorted({ 6, 7,  6,  8, 1, 2, 1, 2, 3,  1,  1,  1,  3,  4,  1,  1,  1,  1,   1,   1});
	std::vector<int32_t> dataIntDSorted({ 1, 2,  3,  4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50,  1,  2,  3,  4,   5,   6});

	std::shared_ptr<Database> database = Database::GetDatabaseByName("WhereEvalDatabase");
	auto& table = database->GetTables().at("TableA");

	//ColA testing for correct sort
	auto& blocksColA = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColA").get())->GetBlocksList();
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

	//ColB testing for correct sort
	auto& blocksColB = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColB").get())->GetBlocksList();
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

	//ColC testing for correct sort
	auto& blocksColC = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColC").get())->GetBlocksList();
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

	//ColD testing for correct sort
	auto& blocksColD = dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("ColD").get())->GetBlocksList();
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

	GpuSqlCustomParser parser(Database::GetDatabaseByName("WhereEvalDatabase"), "SELECT COUNT(ColA) FROM TableA WHERE ColB >= 2 AND ColB <=3 AND ColC>=4 AND ColC <= 8 GROUP BY(ColA);");
	resultPtr = parser.parse();
	LoadColHelper& loadColHelper = LoadColHelper::getInstance();

	ASSERT_EQ(loadColHelper.countSkippedBlocks, 2);

	GpuSqlCustomParser parserDropDatabase(nullptr, "DROP DATABASE WhereEvalDatabase;");
	resultPtr = parserDropDatabase.parse();
}

template<typename T>
struct IdxKeyPair
{
    int32_t index;
    T key;
};

template<typename T>
struct Asc
{
    inline bool operator() (const IdxKeyPair<T>& struct1, const IdxKeyPair<T>& struct2)
    {
        return (struct1.key < struct2.key);
    }
};

template<typename T>
struct Desc
{
    inline bool operator() (const IdxKeyPair<T>& struct1, const IdxKeyPair<T>& struct2)
    {
        return (struct1.key > struct2.key);
    }
};

TEST(DispatcherTests, OrderByTestSimple)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA ORDER BY colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for(int32_t i = 0; i < result->payloads().at("TableA.colInteger1").intpayload().intdata_size(); i++)	
		expectedResultsInt.push_back(result->payloads().at("TableA.colInteger1").intpayload().intdata()[i]);

	std::vector<IdxKeyPair<int32_t>> v(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);

	for (int i = 0, k = 0; i < TEST_BLOCK_COUNT; i++)
		for (int j = 0; j < TEST_BLOCK_SIZE; j++, k++)
			v[k] = {0, columnInt->GetBlocksList()[i]->GetData()[j]};

	stable_sort(v.begin(), v.end(), Asc<int32_t>());

	for (int i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], v[i].key);
	}
}

TEST(DispatcherTests, OrderByTestMulticolumnMultitype)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1, colDouble1 FROM TableA ORDER BY colInteger1 ASC, colLong1 DESC, colFloat1 ASC, colDouble1 DESC;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto dataIn1 = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get())->GetBlocksList();
	auto dataIn2 = dynamic_cast<ColumnBase<int64_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colLong1").get())->GetBlocksList();
	auto dataIn3 = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get())->GetBlocksList();
	auto dataIn4 = dynamic_cast<ColumnBase<double>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colDouble1").get())->GetBlocksList();

	// Get the expected results
	std::vector<int32_t> expectedResultsInt;
	std::vector<double> expectedResultsDouble;

	for(int32_t i = 0; i < result->payloads().at("TableA.colInteger1").intpayload().intdata_size(); i++)	
			expectedResultsInt.push_back(result->payloads().at("TableA.colInteger1").intpayload().intdata()[i]);

	for(int32_t i = 0; i < result->payloads().at("TableA.colDouble1").doublepayload().doubledata_size(); i++)	
			expectedResultsDouble.push_back(result->payloads().at("TableA.colDouble1").doublepayload().doubledata()[i]);
	
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
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
        indices[i] = i;

	// Sort 4th col
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		data4[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
	
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		v4[i] = {indices[i], data4[i]};

	stable_sort(v4.begin(), v4.end(), Desc<double>());

	for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		indices[i] = v4[i].index; 

	// Sort 3th col
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		data3[i] = dataIn3[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
	
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		v3[i] = {indices[i], data3[i]};

	stable_sort(v3.begin(), v3.end(), Asc<float>());

	for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		indices[i] = v3[i].index; 

	// Sort 2th col
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		data2[i] = dataIn2[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
	
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		v2[i] = {indices[i], data2[i]};

	stable_sort(v2.begin(), v2.end(), Desc<int64_t>());

	for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		indices[i] = v2[i].index; 

	// Sort 1th col
	for(int32_t i = 0; i < TEST_BLOCK_COUNT * TEST_BLOCK_SIZE; i++)
			data1[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
	
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		v1[i] = {indices[i], data1[i]};

	stable_sort(v1.begin(), v1.end(), Asc<int32_t>());

	for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
		indices[i] = v1[i].index; 

	
	// Reorder the output data 
	std::vector<int32_t> resultsInt(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
	std::vector<double> resultsDouble(TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
	for(int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
	{
		resultsInt[i] = dataIn1[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
		resultsDouble[i] = dataIn4[indices[i] / TEST_BLOCK_SIZE]->GetData()[indices[i] % TEST_BLOCK_SIZE];
	}

	// Compare the results with the parser results
	for (int32_t i = 0; i < (TEST_BLOCK_COUNT * TEST_BLOCK_SIZE); i++)
	{
		ASSERT_EQ(resultsInt[i] , expectedResultsInt[i]);
		ASSERT_FLOAT_EQ(resultsDouble[i] , expectedResultsDouble[i]);
	}
}

TEST(DispatcherTests, JoinSimpleTest)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA JOIN TableB ON colInteger1 = colInteger3;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::cout << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size() << std::endl;

	auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableB").GetColumns().at("colInteger3").get());

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
	std::vector<int32_t> payloadVector(payloads.intpayload().intdata().begin(), payloads.intpayload().intdata().end());

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA JOIN TableB ON colInteger1 = colInteger3 WHERE colFloat1 < 200;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::cout << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size() << std::endl;

	auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableB").GetColumns().at("colInteger3").get());

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
					if (leftBlockFloat->GetData()[leftRowIdx] < 200 && leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
					{
						expectedResults.push_back(leftBlock->GetData()[leftRowIdx]);
					}
				}
			}
		}
	}

	auto payloads = result->payloads().at("TableA.colInteger1");
	std::vector<int32_t> payloadVector(payloads.intpayload().intdata().begin(), payloads.intpayload().intdata().end());

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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1, COUNT(colInteger1) FROM TableA JOIN TableB ON colInteger1 = colInteger3 GROUP BY colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::cout << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size() << std::endl;

	auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableB").GetColumns().at("colInteger3").get());

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
						if (expectedResults.find(leftBlock->GetData()[leftRowIdx]) == expectedResults.end())
						{
							expectedResults.insert({ leftBlock->GetData()[leftRowIdx], 1 });
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
		ASSERT_EQ(expectedResults.at(payloadsKeys.intpayload().intdata()[i]), payloadsValues.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, JoinGroupByWhereTest)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1, COUNT(colInteger1) FROM TableA JOIN TableB ON colInteger1 = colInteger3 WHERE colFloat1 < 200 GROUP BY colInteger1;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::cout << "Result size: " << result->payloads().at("TableA.colInteger1").intpayload().intdata().size() << std::endl;

	auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableB").GetColumns().at("colInteger3").get());

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
					if (leftBlockFloat->GetData()[leftRowIdx] < 200 && leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
					{
						if (expectedResults.find(leftBlock->GetData()[leftRowIdx]) == expectedResults.end())
						{
							expectedResults.insert({ leftBlock->GetData()[leftRowIdx], 1 });
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
		ASSERT_EQ(expectedResults.at(payloadsKeys.intpayload().intdata()[i]), payloadsValues.int64payload().int64data()[i]);
	}
}

TEST(DispatcherTests, JoinWhereStringTest)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colString1 FROM TableA JOIN TableB ON colInteger1 = colInteger3 WHERE colFloat1 < 200;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::cout << "Result size: " << result->payloads().at("TableA.colString1").stringpayload().stringdata().size() << std::endl;

	auto leftCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());
	auto leftColFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());
	auto leftColString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colString1").get());

	auto rightCol = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableB").GetColumns().at("colInteger3").get());

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
					if (leftBlockFloat->GetData()[leftRowIdx] < 200 && leftBlock->GetData()[leftRowIdx] == rightBlock->GetData()[rightRowIdx])
					{
						expectedResults.push_back(leftBlockString->GetData()[leftRowIdx]);
					}
				}
			}
		}
	}

	auto payloads = result->payloads().at("TableA.colString1");


	std::vector<std::string> payloadVector(payloads.stringpayload().stringdata().begin(), payloads.stringpayload().stringdata().end());

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

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") == DispatcherObjs::GetInstance().database->GetTables().end());

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "CREATE TABLE [tblA%^&*()-+] ([colA%^&*()-+] int, [colB%^&*()-+] float, INDEX [ind%^&*()-+] ([colA%^&*()-+], [colB%^&*()-+]));");
	auto resultPtr = parser.Parse();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") != DispatcherObjs::GetInstance().database->GetTables().end());

	std::vector<std::string> expectedSortingColumns = { "colA%^&*()-+", "colB%^&*()-+" };
	std::vector<std::string> resultSortingColumns = DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetSortingColumns();

	ASSERT_TRUE(expectedSortingColumns.size() == resultSortingColumns.size());

	for (int i = 0; i < expectedSortingColumns.size(); i++)
	{
		ASSERT_TRUE(expectedSortingColumns[i] == resultSortingColumns[i]);
	}

	GpuSqlCustomParser parser2(DispatcherObjs::GetInstance().database, "INSERT INTO [tblA%^&*()-+] ([colA%^&*()-+], [colB%^&*()-+]) VALUES (1, 2.0);");

	for (int32_t i = 0; i < 5; i++)
	{
		resultPtr = parser2.Parse();
	}

	GpuSqlCustomParser parser3(DispatcherObjs::GetInstance().database, "SELECT [colA%^&*()-+], [colB%^&*()-+] from [tblA%^&*()-+];");
	resultPtr = parser3.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsColA;
	std::vector<float> expectedResultsColB;

	for (int k = 0; k < 5; k++)
	{
		expectedResultsColA.push_back(1);
		expectedResultsColB.push_back(2.0);
	}

	auto &payloadsColA = result->payloads().at("tblA%^&*()-+.colA%^&*()-+");
	auto &payloadsColB = result->payloads().at("tblA%^&*()-+.colB%^&*()-+");

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

	GpuSqlCustomParser parser4(DispatcherObjs::GetInstance().database, "ALTER TABLE [tblA%^&*()-+] DROP COLUMN [colA%^&*()-+], ADD [colC%^&*()-+] float;");
	resultPtr = parser4.Parse();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetColumns().find("colA%^&*()-+")
		== DispatcherObjs::GetInstance().database->GetTables().at("tblA%^&*()-+").GetColumns().end());


	GpuSqlCustomParser parser5(DispatcherObjs::GetInstance().database, "SELECT [colB%^&*()-+], [colC%^&*()-+] from [tblA%^&*()-+];");
	resultPtr = parser5.Parse();
	result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsColC;

	for (int k = 0; k < 5; k++)
	{
		expectedResultsColC.push_back(0.0);
	}

	auto &payloadsColB2 = result->payloads().at("tblA%^&*()-+.colB%^&*()-+");
	auto &payloadsColC = result->payloads().at("tblA%^&*()-+.colC%^&*()-+");


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

	GpuSqlCustomParser parser6(DispatcherObjs::GetInstance().database, "DROP TABLE [tblA%^&*()-+];");
	resultPtr = parser6.Parse();

	ASSERT_TRUE(DispatcherObjs::GetInstance().database->GetTables().find("tblA%^&*()-+") == DispatcherObjs::GetInstance().database->GetTables().end());
}

//== Cast Tests ==
TEST(DispatcherTests, CastFloatColToInt)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT CAST(colFloat1 AS INT) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockFloat = columnFloat->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsInt.push_back(static_cast<int32_t>(blockFloat->GetData()[k]));
		}
	}

	auto &payloadsInt = result->payloads().at("CAST(colFloat1ASINT)");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(expectedResultsInt[i], payloadsInt.intpayload().intdata()[i]);
	}
}

TEST(DispatcherTests, CastIntColToFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT CAST(colInteger1 AS FLOAT) FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsFloat.push_back(static_cast<float>(blockInt->GetData()[k]));
		}
	}

	auto &payloadsFloat = result->payloads().at("CAST(colInteger1ASFLOAT)");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(payloadsFloat.floatpayload().floatdata()[i], expectedResultsFloat[i]);
	}
}


TEST(DispatcherTests, AliasWhereSimpleTest)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 - 100 AS result FROM TableA WHERE result > 300;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

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

	auto &payloadsInt = result->payloads().at("result");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), expectedResultsInt.size());

	for (int i = 0; i < payloadsInt.intpayload().intdata_size(); i++)
	{
		ASSERT_EQ(payloadsInt.intpayload().intdata()[i], expectedResultsInt[i]);
	}
}

TEST(DispatcherTests, AllColumnsWithDuplicatesTest)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT *, colInteger1, colFloat1, * FROM TableA;");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	ASSERT_EQ(result->payloads().size(), DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().size());

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
			auto &payloads = result->payloads().at("TableA." + columnName);

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
			auto &payloads = result->payloads().at("TableA." + columnName);

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
			auto &payloads = result->payloads().at("TableA." + columnName);

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
			auto &payloads = result->payloads().at("TableA." + columnName);

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
			auto &payloads = result->payloads().at("TableA." + columnName);
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
			auto &payloads = result->payloads().at("TableA." + columnName);
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
			auto &payloads = result->payloads().at("TableA." + columnName);
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE LEFT(colString1, 4) = \"Word\";");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colString1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockString = columnString->GetBlocksList()[i];
		auto blockFloat = columnFloat->GetBlocksList()[i];

		for (int k = 0; k < (1 << 11); k++)
		{
			if (blockString->GetData()[k].substr(0,4) == "Word")
			{
				expectedResultsFloat.push_back(blockFloat->GetData()[k]);
			}
		}
	}

	auto &payloadsFloat = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, StringConcatLeftWhereColConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE CONCAT(\"Concat\", LEFT(colString1, 4)) = \"ConcatWord\";");
	auto resultPtr = parser.Parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResultsFloat;

	auto columnString = dynamic_cast<ColumnBase<std::string>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colString1").get());
	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

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

	auto &payloadsFloat = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloadsFloat.floatpayload().floatdata_size(), expectedResultsFloat.size());

	for (int i = 0; i < payloadsFloat.floatpayload().floatdata_size(); i++)
	{
		ASSERT_FLOAT_EQ(expectedResultsFloat[i], payloadsFloat.floatpayload().floatdata()[i]);
	}
}
