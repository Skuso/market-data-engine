#include <format>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <cstdlib>
#include <string>

namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>

using tcp = net::ip::tcp;               // from <boost/asio/ip/tcp.hpp>

int main() {

    // connect to: ws-feed.exchange.coinbase.com
    // port: 443


    std::cout << std::format("connecting to: {}:{}\n", "ws-feed.exchange.coinbase.com", 443);

    try {
        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        ssl::context ctx{ssl::context::tlsv12_client};
        ctx.set_default_verify_paths();
        tcp::resolver resolver{ioc};
        websocket::stream<ssl::stream<tcp::socket>> ws{ioc, ctx};
        //         websocket::stream<tcp::socket> ws{ioc};
        

        // Look up the domain name
        auto const results = resolver.resolve("ws-feed.exchange.coinbase.com", "443");

        // Make the connection on the IP address we get from a lookup
        net::connect(beast::get_lowest_layer(ws), results.begin(), results.end());

        SSL_set_tlsext_host_name(ws.next_layer().native_handle(), "ws-feed.exchange.coinbase.com");
        ws.next_layer().handshake(ssl::stream_base::client);

        // Perform the SSL handshake
        ws.handshake("ws-feed.exchange.coinbase.com", "/");

        std::cout << "connected\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
