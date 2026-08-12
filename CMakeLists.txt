cmake_minimum_required(VERSION 3.25)
project(market-data-engine)

find_pacakage(Boost REQUIRED COMPONENTS system thread)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

