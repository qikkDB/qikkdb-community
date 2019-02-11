#include <cstdio>
#include <iostream>
#include <chrono>
#include <boost/log/core.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/trivial.hpp>
#include "QueryEngine/Context.h" 
#include "GpuSqlParser/GpuSqlCustomParser.h"
#include "DatabaseGenerator.h"
#include "Configuration.h"
#include "TCPServer.h"
#include "ClientPoolWorker.h"
#include "TCPClientHandler.h"
#include "ConsoleHandler.h"
#include "QueryEngine/GPUMemoryCache.h"

int main(int argc, char **argv)
{

	Context::getInstance(); // Initialize CUDA context
/*
	boost::log::add_file_log("../log/ColmnarDB.log");
    boost::log::add_console_log(std::cout);
	BOOST_LOG_TRIVIAL(info) << "Starting ColmnarDB...\n";
	Database::LoadDatabasesFromDisk();
	TCPServer<TCPClientHandler, ClientPoolWorker> tcpServer(Configuration::GetInstance().GetListenIP().c_str(), Configuration::GetInstance().GetListenPort());
	RegisterCtrlCHandler(&tcpServer);
	tcpServer.Run();

	Database::SaveAllToDisk();
	BOOST_LOG_TRIVIAL(info) << "Exiting cleanly...";
	
	Context::getInstance().GetCacheForCurrentDevice().getColumn<int32_t>("TestColumn", 1, 32);
	Context::getInstance().GetCacheForCurrentDevice().getColumn<int32_t>("TestColumn", 1, 32);
	Context::getInstance().GetCacheForCurrentDevice().clearCachedBlock("TestColumn", 1);*/

	
	
	std::vector<std::string> tableNames = { "TableA" };
	std::vector<DataType> columnTypes = { {COLUMN_INT}, {COLUMN_INT}/*, {COLUMN_LONG}, {COLUMN_FLOAT}, {COLUMN_POLYGON}, {COLUMN_POINT} */};
	std::shared_ptr<Database> database = DatabaseGenerator::GenerateDatabase("TestDb", 2, 1 << 27, false, tableNames, columnTypes);

	//GPUMemory::hostPin(dynamic_cast<BlockBase<int32_t>&>(*dynamic_cast<ColumnBase<int32_t>&>(*(database->GetTables().at("TableA").GetColumns().at("colInteger"))).GetBlocksList()[0]).GetData().data(), 1 << 24);
	for (int i = 0; i < 4; i++)
	{
		auto start = std::chrono::high_resolution_clock::now();

		GpuSqlCustomParser parser(database, "SELECT COUNT(colInteger1) FROM TableA WHERE colInteger1 <= 20;");

		parser.parse();

		auto end = std::chrono::high_resolution_clock::now();

		std::chrono::duration<double> elapsed(end - start);

		std::cout << "Elapsed time: " << elapsed.count() << " s." << std::endl;
	}
	return 0;
}
