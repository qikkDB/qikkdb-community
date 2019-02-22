#pragma once
#include <memory>
#include <set>
#include <boost/asio.hpp>
#include <optional>
#include "Database.h"

class IClientHandler;

/// <summary>
/// Receiving and dispatching for network requests
/// </summary>
class ITCPWorker
{
protected:
	/// <summary>
	/// Timeout for TCP read and write in ms
	/// </summary>
	int requestTimeout_;
	/// <summary>
	/// Client handled by this instance
	/// </summary>
	boost::asio::ip::tcp::socket socket_;
	/// <summary>
	/// Instance of object responsible for handling messages
	/// </summary>
	std::unique_ptr<IClientHandler> clientHandler_;
	static bool globalQuit_;
public:
	/// <summary>
	/// Create new instance of ITCPWorker object
	/// </summary>
	/// <param name="clientHandler">Instance of object responsible for handling messages</param>
	/// <param name="socket">Client that will be handled by this instance</param>
	/// <param name="requestTimeout">Timeout for TCP read and write in ms</param>
	ITCPWorker(std::unique_ptr<IClientHandler>&& clientHandler, boost::asio::ip::tcp::socket socket, int requestTimeout);
	virtual ~ITCPWorker();
	virtual void HandleClient() = 0;
	virtual void Abort() = 0;
	ITCPWorker(const ITCPWorker&) = delete;
	ITCPWorker& operator=(const ITCPWorker&) = delete;
	static void AbortAllWorkers() { globalQuit_ = true; }
	static void ResetGlobalQuitFlag() { globalQuit_ = false; }
	/// <summary>
	/// Current working database
	/// </summary>
	std::weak_ptr<Database> currentDatabase_;

};
