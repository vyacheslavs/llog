cmake_minimum_required(VERSION 3.10)
project(llog VERSION 0.2 LANGUAGES C CXX)
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
        server/handler_chain.hpp
        server/handler_chain.cpp
        server/lambda_handler.hpp
        server/readline.hpp
        server/readline.cpp
        server/msglog.cpp
        server/msglog.hpp
        server/dqueue.cpp
        server/dqueue.hpp

        client/log.cpp
        client/log.h
)

add_executable(rl
        tests/rl.cpp
)

target_link_libraries(rl PRIVATE readline)
target_link_libraries(llog PRIVATE readline)

target_include_directories(llog PRIVATE ${PROJECT_SOURCE_DIR})
