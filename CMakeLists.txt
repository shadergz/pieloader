cmake_minimum_required(VERSION 3.21)
project(PieLoader)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(pie STATIC)

add_subdirectory(src)
add_subdirectory(test)

