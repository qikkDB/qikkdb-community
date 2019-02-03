#include "ITCPWorker.h"
#include "IClientHandler.h"

bool ITCPWorker::globalQuit_ = false;

ITCPWorker::ITCPWorker(std::unique_ptr<IClientHandler>&& clientHandler, boost::asio::ip::tcp::socket socket, int requestTimeout) 
	: clientHandler_(std::move(clientHandler)),socket_(std::move(socket)),requestTimeout_(requestTimeout)

{
}


ITCPWorker::~ITCPWorker()
{
}

