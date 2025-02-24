cmake_minimum_required(VERSION 3.10)
project(network_program_demo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_COMPILER /usr/bin/clang)
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

add_subdirectory(src)