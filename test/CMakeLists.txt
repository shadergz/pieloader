cmake_minimum_required(VERSION 3.21)

add_executable(loader loader.cpp)
target_link_libraries(loader PRIVATE pie)