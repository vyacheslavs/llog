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
        server/client_messages.hpp
        server/client_messages.cpp
        server/completion/readline_completion_commands_gen.hpp
        server/completion/readline_completion_commands_gen.cpp
        server/completion/readline_severity_completion.hpp
        server/completion/readline_severity_completion.cpp
        server/completion/readline_dedup_completion.cpp
        server/completion/readline_dedup_completion.hpp
        server/completion/readline_completion_handler.cpp
        server/completion/readline_completion_handler.hpp
        server/parser/cmd_parser.cpp
        server/parser/cmd_parser.hpp
        server/parser/cmd_parser_severity.hpp
        server/parser/cmd_parser_severity.cpp
        server/parser/cmd_parser_dedup.hpp
        server/parser/cmd_parser_dedup.cpp
        server/parser/cmd_parser_filterout.cpp

        client/log.cpp
        client/log.h
)

add_executable(rl
        tests/rl.cpp
)

target_link_libraries(rl PRIVATE readline)
target_link_libraries(llog PRIVATE readline)

target_include_directories(llog PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(llog PRIVATE ${PROJECT_SOURCE_DIR}/server)

