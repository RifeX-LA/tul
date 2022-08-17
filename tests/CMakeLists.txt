cmake_minimum_required(VERSION 3.16)
project(tests)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../)

add_executable(Google_Tests_run to_ostream.cpp from_istream.cpp aggregate_size.cpp)
target_link_libraries(Google_Tests_run gtest gtest_main)