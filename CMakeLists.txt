cmake_minimum_required(VERSION 3.10)
project(vk_pixel_reader_layer VERSION 0.2 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(llog
        server/main.cpp
        server/utils.cpp
        server/utils.h
        server/ux_server.cpp
        server/ux_server.hpp
        server/log.cpp
        server/log.hpp
        server/poller.cpp
        server/poller.hpp
        server/ux_connection.hpp
        server/ux_connection.cpp

        client/log.cpp
        client/log.h
)

target_include_directories(llog PRIVATE ${PROJECT_SOURCE_DIR})
