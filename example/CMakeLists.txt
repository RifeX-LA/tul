cmake_minimum_required(VERSION 3.16)
project(utilities)

set(CMAKE_CXX_STANDARD 20)

include_directories(../)

add_executable(utilities main.cpp ../flow/tul/traits.hpp ../flow/tul.hpp)
