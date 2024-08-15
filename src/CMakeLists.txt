cmake_minimum_required(VERSION 3.21)

target_sources(pie PRIVATE
    wavefront.cpp
    wavefront.h
    types.h

    loader.cpp
    include/pie/loader.h
)
target_include_directories(pie PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_include_directories(pie INTERFACE PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/pie>
)
