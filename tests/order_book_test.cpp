#include <catch2/catch_test_macros.hpp>
#include "order_book.h"

namespace {
constexpr Price P1 = 7760000000000;  // 77,600.00
constexpr Price P2 = 7760100000000;  // 77,601.00
constexpr Price P3 = 7760200000000;  // 77,602.00
constexpr Size  S1 = 150000000;      // 1.5
constexpr Size  S2 = 250000000;      // 2.5
}

TEST_CASE("empty book has no best prices") {
    OrderBook book;
    REQUIRE(book.best_bid() == std::nullopt);
    REQUIRE(book.best_ask() == std::nullopt);
    REQUIRE(book.depth(Side::Bid) == 0);
    REQUIRE(book.depth(Side::Ask) == 0);
}

TEST_CASE("a new book is not ready until a snapshot is applied") {
    OrderBook book;
    REQUIRE(book.ready() == false);
}

TEST_CASE("setting a level to size zero erases it") {
    OrderBook book;
    book.set_level(Side::Bid, P1, S1);
    REQUIRE(book.depth(Side::Bid) == 1);

    book.set_level(Side::Bid, P1, 0);
    REQUIRE(book.depth(Side::Bid) == 0);
    REQUIRE(book.size_at(Side::Bid, P1) == std::nullopt);
}

TEST_CASE("setting an existing level replaces its size rather than accumulating") {
    OrderBook book;
    book.set_level(Side::Ask, P2, S1);
    book.set_level(Side::Ask, P2, S2);

    REQUIRE(book.depth(Side::Ask) == 1);
    REQUIRE(book.size_at(Side::Ask, P2) == S2);
}

TEST_CASE("best bid is the highest bid price") {
    OrderBook book;
    book.set_level(Side::Bid, P2, S1);   // inserted out of order
    book.set_level(Side::Bid, P1, S1);
    book.set_level(Side::Bid, P3, S1);

    REQUIRE(book.best_bid() == P3);
    REQUIRE(book.depth(Side::Bid) == 3);
}

TEST_CASE("best ask is the lowest ask price") {
    OrderBook book;
    book.set_level(Side::Ask, P2, S1);
    book.set_level(Side::Ask, P3, S1);
    book.set_level(Side::Ask, P1, S1);

    REQUIRE(book.best_ask() == P1);
    REQUIRE(book.depth(Side::Ask) == 3);
}

TEST_CASE("erasing the best bid promotes the next best") {
    OrderBook book;
    book.set_level(Side::Bid, P1, S1);
    book.set_level(Side::Bid, P2, S1);
    REQUIRE(book.best_bid() == P2);

    book.set_level(Side::Bid, P2, 0);
    REQUIRE(book.best_bid() == P1);
}

TEST_CASE("erasing the best ask promotes the next best") {
    OrderBook book;
    book.set_level(Side::Ask, P1, S1);
    book.set_level(Side::Ask, P2, S1);
    REQUIRE(book.best_ask() == P1);

    book.set_level(Side::Ask, P1, 0);
    REQUIRE(book.best_ask() == P2);
}

TEST_CASE("bids and asks are independent") {
    OrderBook book;
    book.set_level(Side::Bid, P1, S1);
    book.set_level(Side::Ask, P3, S2);

    REQUIRE(book.best_bid() == P1);
    REQUIRE(book.best_ask() == P3);
    REQUIRE(book.size_at(Side::Bid, P3) == std::nullopt);
    REQUIRE(book.size_at(Side::Ask, P1) == std::nullopt);
}

TEST_CASE("size_at reports absence for a price never seen") {
    OrderBook book;
    book.set_level(Side::Bid, P1, S1);
    REQUIRE(book.size_at(Side::Bid, P2) == std::nullopt);
}

TEST_CASE("erasing a level that does not exist is harmless") {
    OrderBook book;
    book.set_level(Side::Bid, P1, 0);
    REQUIRE(book.depth(Side::Bid) == 0);
    REQUIRE(book.best_bid() == std::nullopt);
}

TEST_CASE("clear empties both sides") {
    OrderBook book;
    book.set_level(Side::Bid, P1, S1);
    book.set_level(Side::Ask, P2, S1);

    book.clear();
    REQUIRE(book.depth(Side::Bid) == 0);
    REQUIRE(book.depth(Side::Ask) == 0);
    REQUIRE(book.best_bid() == std::nullopt);
    REQUIRE(book.best_ask() == std::nullopt);
}