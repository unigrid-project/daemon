// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _WEBSOCKET_H_
#define _WEBSOCKET_H_

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <queue>

#include "sync.h"
#include "websocketsignal.h"

#define NUM_WEBSOCKET_THREADS	4

class CWebSocket
{
private:
	CCriticalSection cs_queue[NUM_WEBSOCKET_THREADS];
	std::queue<std::pair<WebSocketSignal, std::string>> queue[NUM_WEBSOCKET_THREADS];

	void fail(boost::system::error_code ec, char const* what);

	void handleMessage(boost::beast::multi_buffer &buffer, int serverIndex);

	void handleConnection(boost::asio::ip::tcp::socket& socket, boost::asio::ssl::context& ctx,
                              int serverIndex, boost::asio::yield_context yield);

	void handleWebSocketThread(boost::asio::io_context& ioc, boost::asio::ssl::context& ctx,
                                   boost::asio::ip::tcp::endpoint endpoint, int serverIndex,
	                           boost::asio::yield_context yield);

public:
	CWebSocket() { /* Empty stub */ }
        void initialize(boost::asio::ip::address& address, unsigned short port);
        void post(WebSocketSignal wss, std::string message);
        void post(WebSocketSignal wss);
};

#endif /* _WEBSOCKET_H_ */
