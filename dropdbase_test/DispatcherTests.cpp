#include <cmath>

#include "gtest/gtest.h"
#include "../dropdbase/DatabaseGenerator.h"
#include "../dropdbase/ColumnBase.h"
#include "../dropdbase/BlockBase.h"
#include "../dropdbase/PointFactory.h"
#include "../dropdbase/ComplexPolygonFactory.h"
#include "../dropdbase/Database.h"
#include "../dropdbase/QueryEngine/Context.h"
#include "../dropdbase/GpuSqlParser/GpuSqlCustomParser.h"
#include "../dropdbase/messages/QueryResponseMessage.pb.h"
#include "DispatcherObjs.h"

/////////////////////
//   ">" operator
/////////////////////

//INT ">"
TEST(DispatcherTests, IntGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG ">"
TEST(DispatcherTests, LongGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 > 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT ">"
TEST(DispatcherTests, FloatGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 > 5.5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) > 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((float)(j % 1024 + 0.1111) < 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if ((float)((j % 1024 + 0.1111) * -1) < 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * -1));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 2048 + 0.1111)) > ((float)(j % 1024 + 0.1111)))
				{
					expectedResult.push_back((float)(j % 2048 + 0.1111));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE ">"
TEST(DispatcherTests, DoubleGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 > 5.5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   "<" operator
/////////////////////

//INT "<"
TEST(DispatcherTests, IntLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 < 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG "<"
TEST(DispatcherTests, LongLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 < 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT "<"
TEST(DispatcherTests, FloatLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 < 5.5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((float)(j % 1024 + 0.1111) < 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if ((float)((j % 1024 + 0.1111) * -1) < 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * -1));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((float)(j % 1024 + 0.1111) > 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if ((float)((j % 1024 + 0.1111) * -1) > 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * -1));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) < ((float)(j % 2048 + 0.1111)))
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) < ((float)((j % 2048 + 0.1111) * (-1))))
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE "<"
TEST(DispatcherTests, DoubleLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 < 5.5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   ">=" operator
/////////////////////

//INT ">="
TEST(DispatcherTests, IntEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 >= 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG ">="
TEST(DispatcherTests, LongEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 >= 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT ">="
TEST(DispatcherTests, FloatEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 >= 5.5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) >= 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) >= 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) <= 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) <= 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 2048 + 0.1111)) >= ((float)(j % 1024 + 0.1111)))
				{
					expectedResult.push_back((float)(j % 2048 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 2048 + 0.1111) * (-1))) >= ((float)((j % 1024 + 0.1111) * (-1))))
				{
					expectedResult.push_back((float)((j % 2048 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE ">="
TEST(DispatcherTests, DoubleEqGtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 >= 5.5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   "<=" operator
/////////////////////

//INT "<="
TEST(DispatcherTests, IntEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 <= 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG "<="
TEST(DispatcherTests, LongEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 <= 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT "<="
TEST(DispatcherTests, FloatEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 <= 5.5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) <= 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) <= 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) >= 5.5)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) >= 5.5)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (((float)(j % 1024 + 0.1111)) <= ((float)(j % 2048 + 0.1111)))
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if (((float)((j % 1024 + 0.1111) * (-1))) <= ((float)((j % 2048 + 0.1111) * (-1))))
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE "<="
TEST(DispatcherTests, DoubleEqLtColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 <= 5.5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   "=" operator
/////////////////////

//INT "="
TEST(DispatcherTests, IntEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 = 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG "="
TEST(DispatcherTests, LongEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 = 500000000;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024 == 500000000)
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

TEST(DispatcherTests, LongEqConstColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 = colLong1;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (500000000 == static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024)
			{
				(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
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

TEST(DispatcherTests, LongEqColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong2 FROM TableA WHERE colLong2 = colLong1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT "="
TEST(DispatcherTests, FloatEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 = 5.1111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs(((float)(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs(((float)(j % 1024 + 0.1111)) - 5.1111) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs(((float)(j % 2048 + 0.1111)) - ((float)(j % 1024 + 0.1111))) < 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 2048 + 0.1111)) : expectedResult.push_back((float)((j % 2048 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE "="
/*TEST(DispatcherTests, DoubleEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 = 5.1111111;");
	auto resultPtr = parser.parse();
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.1111111 = colDouble1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   "!=" operator
/////////////////////

//INT "!="
TEST(DispatcherTests, IntNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 != 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG "!="
TEST(DispatcherTests, LongNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 != 50000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.int64payload().int64data_size(), 0);
}

//FLOAT "!="
TEST(DispatcherTests, FloatNotEqColumnConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 != 5.1111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs(((float)(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
			if (std::abs(((float)(j % 1024 + 0.1111)) - 5.1111) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (std::abs(((float)(j % 2048 + 0.1111))-((float)(j % 1024 + 0.1111))) > 0.00005)
			{
				(j % 2) ? expectedResult.push_back((float)(j % 2048 + 0.1111)) : expectedResult.push_back((float)((j % 2048 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE "!="
/*TEST(DispatcherTests, DoubleNotEqColumnConst) //FIXME test je dobry, chyba je v kerneli
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 != 5.1111111;");
	auto resultPtr = parser.parse();
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

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.1111111 != colDouble1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   AND
/////////////////////

//INT AND
TEST(DispatcherTests, IntAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 AND 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, IntAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 5 AND colInteger1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, IntAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 AND colInteger2;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 AND 5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, IntAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG AND
TEST(DispatcherTests, LongAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 AND 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}
TEST(DispatcherTests, LongAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 500000000 AND colLong1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LongAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 AND colLong2;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 AND 5;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LongAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

//FLOAT AND
TEST(DispatcherTests, FloatAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 AND 5.1111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

TEST(DispatcherTests, FloatAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 5.1111 AND colFloat1;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

TEST(DispatcherTests, FloatAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat2 AND colFloat1;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) && ((float)(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back((float)(j %1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));

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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 AND 5.1111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

TEST(DispatcherTests, FloatAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE AND
TEST(DispatcherTests, DoubleAndColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 AND 5.1111111;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

TEST(DispatcherTests, DoubleAndConstColumnNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 5.1111111 AND colDouble1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

TEST(DispatcherTests, DoubleAndColumnColumn)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble2 AND colDouble1;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseLeftZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 AND 5.11111111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

TEST(DispatcherTests, DoubleAndConstConstFalseBothZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE 0 AND 0;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}


/////////////////////
//   OR
/////////////////////

//INT OR
TEST(DispatcherTests, IntOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 OR 5;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

// LONG OR
TEST(DispatcherTests, LongOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 OR 500000000;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colLong1");

	ASSERT_EQ(payloads.intpayload().intdata_size(), 0);
}

//FLOAT OR
TEST(DispatcherTests, FloatOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 OR 5.1111;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 1024 + 0.1111)) > std::numeric_limits<float>::epsilon())
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (((float)(j % 2048 + 0.1111) > std::numeric_limits<float>::epsilon()) || ((float)(j % 1024 + 0.1111) > std::numeric_limits<float>::epsilon()))
			{
				(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			(j % 2) ? expectedResult.push_back((float)(j % 1024 + 0.1111)) : expectedResult.push_back((float)((j % 1024 + 0.1111) * (-1)));
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colFloat1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), 0);
}

//DOUBLE OR
TEST(DispatcherTests, DoubleOrColumnConstNonZero)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 OR 5.1111111;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	auto &payloads = result->payloads().at("TableA.colDouble1");

	ASSERT_EQ(payloads.doublepayload().doubledata_size(), 0);
}

/////////////////////
//   NEGATION
/////////////////////

// INT NEGATION
TEST(DispatcherTests, IntNegation)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE !(colInteger1 > 5);");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if ((float)(j % 1024 + 0.1111) <= 6.5555)
				{
					expectedResult.push_back((float)(j % 1024 + 0.1111));
				}
			}
			else
			{
				if ((float)((j % 1024 + 0.1111) * -1) <= 6.5555)
				{
					expectedResult.push_back((float)((j % 1024 + 0.1111) * -1));
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (j % 2)
			{
				if (static_cast<int32_t>(j % 1024) / 5 > 500)
				{
					expectedResult.push_back(j % 1024);
				}
			}
			else
			{
				if ((static_cast<int32_t>(j % 1024) * -1) / 5 > 500)
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

TEST(DispatcherTests, IntDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<float> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((((j % 1024) + 0.1111) / 5) > 500)
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

TEST(DispatcherTests, FloatDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colFloat1 FROM TableA WHERE colFloat1 / 5 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<double> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((((j % 1024) + 0.1111111) / 5) > 500)
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

TEST(DispatcherTests, DoubleDivColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colDouble1 FROM TableA WHERE colDouble1 / 5 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if ((j % 1024) / 5.0 > 500)
			{
				(j % 2) ? expectedResult.push_back(j % 1024) : expectedResult.push_back((j % 1024) * ((-1)));
			}
		}
	}

	auto &payloads = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloads.floatpayload().floatdata_size(), expectedResult.size());

	for (int i = 0; i < payloads.floatpayload().floatdata_size(); i++)
	{
		ASSERT_EQ(expectedResult[i], payloads.floatpayload().floatdata()[i]);
	}
}

TEST(DispatcherTests, IntDivColumnConstLtConstFloat)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 / 5.0 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (static_cast<int32_t>((j % 1024) % 5) > 500)
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

TEST(DispatcherTests, IntModColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 % 5 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int64_t> expectedResult;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < (1 << 11); j++)
		{
			if (static_cast<int64_t>(((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) % 5)) > 500)
			{
				(j % 2) ? expectedResult.push_back(static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) : expectedResult.push_back((static_cast<int64_t>(2 * pow(10, j % 19)) + j % 1024) * -1);
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

TEST(DispatcherTests, LongModColumnConstLtConst)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colLong1 FROM TableA WHERE colLong1 % 5 < 500;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPolygon1 FROM TableA;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPolygon1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPoint1 FROM TableA;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto column = dynamic_cast<ColumnBase<ColmnarDB::Types::Point>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colPoint1").get());

	for (int i = 0; i < 2; i++)
	{
		auto block = column->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k]));
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colPoint1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.parse();
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
					expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k]));
				}
			}
			else
			{
				if (((k % 1024) * -1) < 20)
				{
					expectedResultsPoints.push_back(PointFactory::WktFromPoint(block->GetData()[k]));
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colString1 FROM TableA;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colString1 FROM TableA WHERE colInteger1 < 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(colInteger1, colFloat1) FROM TableA;");
	auto resultPtr = parser.parse();
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
			wktStream << "POINT(" << blockInt->GetData()[k] << " " << blockFloat->GetData()[k] << ")";
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(colInteger1, 4.5) FROM TableA;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			std::ostringstream wktStream;
			wktStream << "POINT(" << blockInt->GetData()[k] << " 4.5" << ")";
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT POINT(7, colFloat1) FROM TableA;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;

	auto columnFloat = dynamic_cast<ColumnBase<float>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colFloat1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockFloat = columnFloat->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			std::ostringstream wktStream;
			wktStream << "POINT(" << "7 " << blockFloat->GetData()[k] << ")";
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	auto &payloads = result->payloads().at("COUNT(colInteger1)");
	ASSERT_EQ(payloads.int64payload().int64data_size(), 1);
	ASSERT_EQ(payloads.int64payload().int64data()[0], TEST_BLOCK_COUNT * TEST_BLOCK_SIZE);
}


TEST(DispatcherTests, Alias)
{
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT (t.colInteger1 - 10) AS col1, t.colFloat1 AS col2 FROM TableA as t WHERE t.colInteger1 > 20;");
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 OFFSET 10000000;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());


	auto &payloadsInt = result->payloads().at("TableA.colInteger1");

	ASSERT_EQ(payloadsInt.intpayload().intdata_size(), 0);
}

TEST(DispatcherTests, LargeLimit)
{
	Context::getInstance();
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 > 20 LIMIT 10000000;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 | 20) > 100;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			if ((blockInt->GetData()[k] | 20) > 100)
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 & 20) > 100;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			if ((blockInt->GetData()[k] & 20) > 100)
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ 20) > 100;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 << 2) > 100;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 >> 2) > 100;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 | colInteger2) > 500;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 & colInteger2) > 10;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE (colInteger1 ^ colInteger2) > 500;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE colInteger1 <> 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE -colInteger1 = 3;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];
		for (int k = 0; k < (1 << 11); k++)
		{
			if (-blockInt->GetData()[k] == 3)
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

TEST(DispatcherTests, AbsColInt)
{
	Context::getInstance();
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ABS(colInteger1) = 3;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colInteger1) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE COS(colInteger1) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE TAN(colInteger1) > 2;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE COS(colFloat1) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE TAN(colFloat1) > 2;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIN(colFloat1 + PI()) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ASIN(colInteger1 / 1024.0) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ACOS(colInteger1 / 1024.0) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ATAN(colInteger1) > 0.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG10(colInteger1) > 1.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1) > 1.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE LOG(colInteger1, 3.0) > 1.5;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE EXP(colInteger1) > 2000;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE POW(colInteger1, 2) > 2000;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SQRT(colInteger1) > 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SQUARE(colInteger1) > 2000;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 1;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = -1;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE SIGN(colInteger1) = 0;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT colInteger1 FROM TableA WHERE ROOT(colInteger1, 3) > 20;");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<int32_t> expectedResultsInt;

	auto columnInt = dynamic_cast<ColumnBase<int32_t>*>(DispatcherObjs::GetInstance().database->GetTables().at("TableA").GetColumns().at("colInteger1").get());

	for (int i = 0; i < 2; i++)
	{
		auto blockInt = columnInt->GetBlocksList()[i];

		for (int k = 0; k < (1 << 11); k++)
		{
			if (pow(blockInt->GetData()[k], 1/3) > 20)
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


TEST(DispatcherTests, RoundColFloat)
{
	Context::getInstance();
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ROUND(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT FLOOR(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT CEIL(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ROUND(colInteger1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT COT(colFloat1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT ATAN2(colFloat1, colFloat1 + 1) FROM TableA WHERE colInteger1 >= 20;");
	auto resultPtr = parser.parse();
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

/// Run query SELECT function(column) FROM table; and return result payload
ColmnarDB::NetworkClient::Message::QueryResponsePayload StringFunctionHelp(
	std::string function, std::string column, std::string table)
{
	std::string retFunCol = function + "(" + column + ")";
	Context::getInstance();

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database, "SELECT " + retFunCol + " FROM " + table + ";");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());
	return result->payloads().at(retFunCol);
}

TEST(DispatcherTests, StringLower)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto &payloads = StringFunctionHelp("LOWER", col, table);

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

TEST(DispatcherTests, StringUpper)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto &payloads = StringFunctionHelp("UPPER", col, table);

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

TEST(DispatcherTests, StringReverse)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto &payloads = StringFunctionHelp("REVERSE", col, table);

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

TEST(DispatcherTests, StringLtrim)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto &payloads = StringFunctionHelp("LTRIM", col, table);

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

TEST(DispatcherTests, StringRtrim)
{
	const std::string col = "colString1";
	const std::string table = "TableA";
	auto &payloads = StringFunctionHelp("RTRIM", col, table);

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

// Polygon clipping tests
/*
// TODO: fix zero allocation, finish polygon clippin and add asserts
TEST(DispatcherTests, PolygonClippingAndContains)
{
	Context::getInstance();
	int32_t polygonColumnCount = 1;

	GpuSqlCustomParser parser(DispatcherObjs::GetInstance().database,
		"SELECT colInteger1 FROM TableA WHERE GEO_CONTAINS(GEO_INTERSECT(colPolygon1, colPolygon2), colPoint1);");
	auto resultPtr = parser.parse();
	auto result = dynamic_cast<ColmnarDB::NetworkClient::Message::QueryResponseMessage*>(resultPtr.get());

	std::vector<std::string> expectedResultsPoints;
}
*/
