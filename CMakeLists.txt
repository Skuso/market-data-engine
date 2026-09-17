cmake_minimum_required(VERSION 3.25)
project(market-data-engine CXX)

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED COMPONENTS Crypto SSL)
find_package(nlohmann_json REQUIRED)
find_package(Catch2 REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(market_data_core src/order_book.cpp)
target_include_directories(market_data_core PUBLIC src)
target_compile_features(market_data_core PUBLIC cxx_std_20)
target_compile_options(market_data_core PRIVATE -Wall -Wextra -Wpedantic)


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE market_data_core Boost::headers OpenSSL::Crypto OpenSSL::SSL nlohmann_json::nlohmann_json)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

add_executable(order_book_test tests/order_book_test.cpp)
target_link_libraries(order_book_test PRIVATE market_data_core Catch2::Catch2WithMain)
target_compile_options(order_book_test PRIVATE -Wall -Wextra -Wpedantic)



