cmake_minimum_required(VERSION 3.16)
project(yazi-thread)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(./)

file(GLOB_RECURSE SOURCES "util/*.cpp" "thread/*.cpp" "task/*.cpp")

add_executable(main ${SOURCES} main.cpp)
