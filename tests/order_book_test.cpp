#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_session.hpp>
#include "order_book.h"

int main(int argc, char* argv[]) {
    order_book_test::Session session;
    return session.run(argc, argv);
}