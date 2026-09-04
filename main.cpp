#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/websocket.hpp>
#include <charconv>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

namespace ssl = boost::asio::ssl;        // from <boost/asio/ssl.hpp>
namespace beast = boost::beast;          // from <boost/beast.hpp>
namespace http = beast::http;            // from <boost/beast/http.hpp>
namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>
namespace net = boost::asio;             // from <boost/asio.hpp>

using tcp = net::ip::tcp;    // from <boost/asio/ip/tcp.hpp>

using Price = std::int64_t;
using Size = std::int64_t;
constexpr std::int64_t PRICE_SCALE = 100'000'000;

/**
 * Parses a string representing a fixed-point decimal number into an integer representation scaled by PRICE_SCALE.
 *
 * @instring The input string to parse.
 * @return An optional containing the parsed integer value, or std::nullopt if parsing fails.
 */

std::optional<std::int64_t> parse_fixed_point(const std::string& str) {
    if (str.empty()) return std::nullopt;

    // Policy: Prices and sizes in this crypto feed can never be negative.
    if (str.front() == '-') return std::nullopt;

    auto decimal_pos = str.find('.');

    if (decimal_pos == std::string::npos) { // No decimal point, treat as integer
        std::int64_t value = 0;
        auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
        // Ensure successful parse AND that no trailing garbage ("5abc") was left behind
        if (ec != std::errc{} || ptr != str.data() + str.size()) return std::nullopt;
        return value * PRICE_SCALE;
    } 
    
    std::string integer_part = str.substr(0, decimal_pos);
    std::string fractional_part = str.substr(decimal_pos + 1);

    // Policy: Truncate fractions beyond 8 decimal places.
    if (fractional_part.length() > 8) {
        fractional_part = fractional_part.substr(0, 8);
    } else {
        // Pad with zeros to ensure exact scaling
        fractional_part.append(8 - fractional_part.length(), '0');
    }

    std::int64_t int_val = 0;
    if (!integer_part.empty()) { 
        auto [ptr, ec] = std::from_chars(integer_part.data(), integer_part.data() + integer_part.size(), int_val);
        if (ec != std::errc{} || ptr != integer_part.data() + integer_part.size()) return std::nullopt;
    }

    std::int64_t frac_val = 0;
    auto [ptr, ec] = std::from_chars(fractional_part.data(), fractional_part.data() + fractional_part.size(), frac_val);
    if (ec != std::errc{} || ptr != fractional_part.data() + fractional_part.size()) return std::nullopt;

    return (int_val * PRICE_SCALE) + frac_val;
}

/**
 * Formats a Price value into a string representation with 8 decimal places.
 *
 * @param price The Price value to format.
 * @return A string representing the formatted price.
 */
std::string format_price(Price price) {
   return std::format("{}.{:08}", price / PRICE_SCALE, price % PRICE_SCALE);
}

/**
 * The main function establishes a secure WebSocket connection to the Coinbase exchange feed,
 * subscribes to the BTC-USD ticker channel, and continuously reads and processes match messages.
 *
 * @return EXIT_SUCCESS on successful execution, or EXIT_FAILURE on error.
 */
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
        {"channels", {{{"name", "matches"}, {"product_ids", {"BTC-USD"}}}}}};

    std::string subscribe_message_str = subscribe_message.dump();

    ws.write(net::buffer(subscribe_message_str));

    beast::flat_buffer buffer;
    while (true) {
      ws.read(buffer);

      // Convert to string
      std::string message = beast::buffers_to_string(buffer.data());

      // Consume buffer immediately so it is empty for the next iteration
      buffer.consume(buffer.size());

      // Parse JSON safely without throwing
      auto j = nlohmann::json::parse(message, nullptr, false);

      // Discarded? Skip.
      if (j.is_discarded()) {
        std::cerr << "Skip: Malformed JSON\n";
        continue;
      }

      // Not a match? Skip.
      if (!j.contains("type") || j["type"] != "match") {
        continue;
      }

      if (!j.contains("price") || !j.contains("size")) {
        std::cerr << "Skip: Match message missing price or size\n";
        continue;
      }

      // Extract strings and parse
      auto price_opt = parse_fixed_point(j["price"].get<std::string>());
      auto size_opt = parse_fixed_point(j["size"].get<std::string>());

      // Empty optional? Log and skip.
      if (!price_opt || !size_opt) {
        std::cerr << "Skip: Failed to parse price or size -> " << message
                  << "\n";
        continue;
      }

      // Unpack and use
      Price price = *price_opt;
      Size size = *size_opt;

      std::cout << std::format("Matched - Price: {}, Size: {}\n", format_price(price), size);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
