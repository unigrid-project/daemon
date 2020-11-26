// Copyright (c) 2020 The UNIGRID organization
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <functional>
#include <queue>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include "chainparamsbase.h"
#include "websocket.h"

/*
	boost::asio::ip::make_address("127.0.0.1")
	BaseParams().WebSocketPort()
*/
void CWebSocket::initialize(boost::asio::ip::address& address, unsigned short port)
{
	boost::asio::io_context ioc[NUM_WEBSOCKET_THREADS];

	// Holds certificates
	boost::asio::ssl::context ctx{boost::asio::ssl::context::tlsv12};

	// Self-sign certificate
	// load_server_certificate(ctx); //TODO: Enable me!

	// Run the I/O service on the requested number of threads
	std::vector<std::thread> v;

	for (int i = 0; i < NUM_WEBSOCKET_THREADS; i++) {
		boost::asio::spawn(ioc[i],
			std::bind(
				&CWebSocket::handleWebSocketThread,
				this,
				std::ref(ioc[i]),
				std::ref(ctx),
				boost::asio::ip::tcp::endpoint {address, port},
				i,
				std::placeholders::_1
			)
		);

		v.emplace_back([&ioc, i] {
			ioc[i].run();
		});
        }
}

void CWebSocket::fail(boost::system::error_code ec, char const* what)
{
    std::cerr << what << ": " << ec.message() << "\n";
}

void CWebSocket::handleMessage(boost::beast::multi_buffer &buffer, int serverIndex)
{
	LOCK(cs_queue[serverIndex]);

	boost::beast::ostream(buffer) << queue[serverIndex].front().first;
	boost::beast::ostream(buffer) << queue[serverIndex].front().second;
	queue[serverIndex].pop();
}

void CWebSocket::handleConnection(boost::asio::ip::tcp::socket& socket, boost::asio::ssl::context& ctx,
                                  int serverIndex, boost::asio::yield_context yield)
{
	boost::beast::error_code ec;
	boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket&>> ws{socket, ctx};

	// SSL handshake
	ws.next_layer().async_handshake(boost::asio::ssl::stream_base::server, yield[ec]);

	if (ec) {
	        return fail(ec, "handshake");
	}

	// Accept the websocket connection
	ws.async_accept(yield[ec]);

	if(ec) {
		return fail(ec, "accept");
	}

	while (true) {
		boost::beast::multi_buffer buffer;

		handleMessage(buffer, serverIndex);
		ws.async_write(buffer.data(), yield[ec]);
		buffer.consume(buffer.size());

		if(ec) {
			return fail(ec, "write");
		}
	}
}

void CWebSocket::handleWebSocketThread(boost::asio::io_context& ioc, boost::asio::ssl::context& ctx,
                                         boost::asio::ip::tcp::endpoint endpoint, int serverIndex, boost::asio::yield_context yield)
{
	boost::beast::error_code ec;
	boost::asio::ip::tcp::acceptor acceptor(ioc);
	acceptor.open(endpoint.protocol(), ec);

	if (ec) {
		return fail(ec, "open");
	}

	// Allow address reuse
	acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);

	if (ec) {
	        return fail(ec, "set_option");
	}

	acceptor.bind(endpoint, ec);

	if (ec) {
		return fail(ec, "bind");
	}

	acceptor.listen(boost::asio::socket_base::max_listen_connections, ec);

	if (ec) {
		return fail(ec, "listen");
	}


	while (true)
	{
		boost::asio::ip::tcp::socket socket(ioc);
		acceptor.async_accept(socket, yield[ec]);

		if(ec) {
			fail(ec, "accept");
		} else {
			boost::asio::spawn(
				acceptor.get_executor(),
				std::bind(
					&CWebSocket::handleConnection,
					this,
					std::move(socket),
					std::ref(ctx),
					serverIndex,
					std::placeholders::_1
				)
			);
		}
	}
}

void CWebSocket::post(WebSocketSignal wss, std::string message)
{
	for (int i = 0; i < NUM_WEBSOCKET_THREADS; i++) {
		{
			LOCK(cs_queue[i]);
			queue[i].push(std::make_pair(wss, message));
		}
	}
}

void CWebSocket::post(WebSocketSignal wss)
{
	post(wss, std::string());
}
