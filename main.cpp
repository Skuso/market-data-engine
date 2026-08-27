#include <format>
#include <iostream>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/ssl.hpp>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <cstdint>
#include <string>

namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>

using tcp = net::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
using Price = std::int64_t; // Price is represented as an integer scaled by PRICE_SCALE to avoid floating-point inaccuracies
using Size = std::int64_t; // Size is represented as an integer scaled by PRICE_SCALE to avoid floating-point inaccuracies

constexpr int64_t PRICE_SCALE  = 100'000'000; // 1e8

struct Values { // Structure to hold price and size values
    Price price;
    Size size;
};

Price parse_price(const std::string& price_str) { // Parse a price string into a Price type while avoiding floating-point inaccuracies
    
    

}

int main() {

    // The host and port we want to connect to
    const std::string host = "ws-feed.exchange.coinbase.com";
    const int port = 443;


    std::cout << std::format("connecting to: {}:{}\n", host, port);

    try {
        // The io_context is required for all I/O
        net::io_context ioc;

        // These objects perform our I/O
        ssl::context ctx{ssl::context::tlsv12_client};
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_peer);
        tcp::resolver resolver{ioc};
        websocket::stream<ssl::stream<tcp::socket>> ws{ioc, ctx};
        //         websocket::stream<tcp::socket> ws{ioc};
        

        // Look up the domain name
        auto const results = resolver.resolve(host, std::to_string(port));

        // Make the connection on the IP address we get from a lookup
        net::connect(beast::get_lowest_layer(ws), results.begin(), results.end());

        SSL_set_tlsext_host_name(ws.next_layer().native_handle(), host.c_str());

        // Perform the SSL handshake
        ws.next_layer().handshake(ssl::stream_base::client);
        ws.handshake(host, "/");

        std::cout << "connected\n";

        // subscribe to the BTC-USD ticker channel
        nlohmann::json subscribe_message = {
            {"type", "subscribe"},
            {"channels", {
                {
                    {"name", "matches"},
                    {"product_ids", {"BTC-USD"}}
                }
            }}
        };

        std::string subscribe_message_str = subscribe_message.dump();

        ws.write(net::buffer(subscribe_message_str));

        beast::flat_buffer buffer;
        while (true) {
            ws.read(buffer);
            std::string message = beast::buffers_to_string(buffer.data());
            std::cout << "Received: " << message << std::endl;
            buffer.consume(buffer.size());
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    



    return EXIT_SUCCESS;
}
