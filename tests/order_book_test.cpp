#include <catch2/catch_test_macros.hpp>
#include "order_book.h"

TEST_CASE("setting a level to size zero erases it") {
    OrderBook book;

    book.set_level(Side::Bid, 7760000000000, 150000000);
    REQUIRE(book.depth(Side::Bid) == 1);

    book.set_level(Side::Bid, 7760000000000, 0);
    REQUIRE(book.depth(Side::Bid) == 0);
    REQUIRE(book.size_at(Side::Bid, 7760000000000) == std::nullopt);
}