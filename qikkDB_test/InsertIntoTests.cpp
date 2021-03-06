#include "gtest/gtest.h"
#include "../qikkDB/DataType.h"
#include "../qikkDB/Database.h"
#include "../qikkDB/Table.h"
#include "../qikkDB/DatabaseGenerator.h"
#include "../qikkDB/GpuSqlParser/GpuSqlCustomParser.h"
#include "../qikkDB/ColumnBase.h"
#include "../qikkDB/PointFactory.h"
#include "../qikkDB/ComplexPolygonFactory.h"
#include "../qikkDB/GpuSqlParser/ParserExceptions.h"

TEST(InsertIntoTests, InsertIntoCorrect)
{
	Database::RemoveFromInMemoryDatabaseList("TestDb");
	int blockSize = 1 << 5;

	std::vector<std::string> tableNames = {"TableA"};
	std::vector<DataType> columnTypes = { COLUMN_INT, COLUMN_LONG, COLUMN_FLOAT, COLUMN_POLYGON, COLUMN_POINT};
	std::shared_ptr<Database> database = DatabaseGenerator::GenerateDatabase("TestDb", 1, blockSize, true, tableNames, columnTypes);
	Database::AddToInMemoryDatabaseList(database);

	GpuSqlCustomParser parser(database, "INSERT INTO TableA (colInteger1, colLong1, colFloat1, colPolygon1, colPoint1) VALUES (500,20000000, 2.5, POLYGON((20 15, 11 12, 20 15),(21 30, 35 36, 30 20, 21 30),(61 80,90 89,112 110, 61 80)), POINT(2 5));");
    parser.Parse();
	auto& table = database->GetTables().at("TableA");
	
	std::vector<int32_t> dataInIntBlock;
	std::vector<int64_t> dataInLongBlock;
	std::vector<float> dataInFloatBlock;
	std::vector<double> dataInDoubleBlock;
	std::vector<QikkDB::Types::Point> dataInPointBlock;
	std::vector<QikkDB::Types::ComplexPolygon> dataInPolygonBlock;
	std::vector<std::string> dataInStringBlock;

	for (auto &block : dynamic_cast<ColumnBase<int32_t>*>(table.GetColumns().at("colInteger1").get())->GetBlocksList())
	{
		for(int i = 0; i < block->GetSize(); i++)
		{
			dataInIntBlock.push_back(block->GetData()[i]);
		}
	}

	for (auto &block : dynamic_cast<ColumnBase<int64_t>*>(table.GetColumns().at("colLong1").get())->GetBlocksList())
	{
		for (int i = 0; i < block->GetSize(); i++)
		{
			dataInLongBlock.push_back(block->GetData()[i]);
		}
	}

	for (auto &block : dynamic_cast<ColumnBase<float>*>(table.GetColumns().at("colFloat1").get())->GetBlocksList())
	{
		for (int i = 0; i < block->GetSize(); i++)
		{
			dataInFloatBlock.push_back(block->GetData()[i]);
		}
	}

	for (auto &block : dynamic_cast<ColumnBase<QikkDB::Types::Point>*>(table.GetColumns().at("colPoint1").get())->GetBlocksList())
	{
		for (int i = 0; i < block->GetSize(); i++)
		{
			dataInPointBlock.push_back(block->GetData()[i]);
		}
	}

	for (auto &block : dynamic_cast<ColumnBase<QikkDB::Types::ComplexPolygon>*>(table.GetColumns().at("colPolygon1").get())->GetBlocksList())
	{
		for (int i = 0; i < block->GetSize(); i++)
		{
			dataInPolygonBlock.push_back(block->GetData()[i]);
		}
	}

	QikkDB::Types::Point addedPoint = PointFactory::FromWkt("POINT(2 5)");
	QikkDB::Types::ComplexPolygon addedPolygon = ComplexPolygonFactory::FromWkt("POLYGON((20 15, 11 12, 20 15), (21 30, 35 36, 30 20, 21 30), (61 80, 90 89, 112 110, 61 80))");

	ASSERT_EQ(500, dataInIntBlock[blockSize]);
	ASSERT_EQ(20000000, dataInLongBlock[blockSize]);
	ASSERT_FLOAT_EQ((float)2.5, dataInFloatBlock[blockSize]);
	ASSERT_EQ(PointFactory::WktFromPoint(addedPoint), PointFactory::WktFromPoint(dataInPointBlock[blockSize]));
	ASSERT_EQ(ComplexPolygonFactory::WktFromPolygon(addedPolygon), ComplexPolygonFactory::WktFromPolygon(dataInPolygonBlock[blockSize]));
	Database::RemoveFromInMemoryDatabaseList("TestDb");
}

TEST(InsertIntoTests, InsertIntoTableNotFound)
{
	Database::RemoveFromInMemoryDatabaseList("TestDb");
	int blockSize = 1 << 5;

	std::vector<std::string> tableNames = {"TableA"};
	std::vector<DataType> columnTypes = {COLUMN_INT, COLUMN_LONG, COLUMN_FLOAT, COLUMN_POLYGON, COLUMN_POINT };
	std::shared_ptr<Database> database = DatabaseGenerator::GenerateDatabase("TestDb", 1, blockSize, true, tableNames, columnTypes);
	Database::AddToInMemoryDatabaseList(database);
	
	GpuSqlCustomParser parser(database, "INSERT INTO TableB (colInteger1, colLong1, colFloat1, colPolygon1, colPoint1) VALUES (500,20000000, 2.5, POLYGON((20 15, 11 12, 20 15),(21 30, 35 36, 30 20, 21 30),(61 80,90 89,112 110, 61 80)), POINT(2 5));");

	ASSERT_THROW(parser.Parse(), TableNotFoundFromException);
	Database::RemoveFromInMemoryDatabaseList("TestDb");
}

TEST(InsertIntoTests, InsertIntoTableNullValue)
{
	Database::RemoveFromInMemoryDatabaseList("TestDb");
	int blockSize = 1 << 5;
	std::shared_ptr<Database> database(std::make_shared<Database>("TestDb"));
	Database::AddToInMemoryDatabaseList(database);
	std::unordered_map<std::string, DataType> columns;
	columns.emplace("Col1",COLUMN_INT);
	database->CreateTable(columns,"TestTable");
	for(int i = 0; i < 16; i++)
	{
		if(i % 2 == i/8)
		{
			GpuSqlCustomParser parser(database, "INSERT INTO TestTable (Col1) VALUES (null);");
            parser.Parse();
		}
		else
		{
			GpuSqlCustomParser parser(database, "INSERT INTO TestTable (Col1) VALUES (1);");
            parser.Parse();
		}
	}
	auto& blockList = dynamic_cast<ColumnBase<int32_t>*>(database->GetTables().at("TestTable").GetColumns().at("Col1").get())->GetBlocksList();
	auto maskPtr = blockList[0]->GetNullBitmask();
	auto val = maskPtr[0]; 
	ASSERT_EQ(val, 43605);
	Database::RemoveFromInMemoryDatabaseList("TestDb");
}