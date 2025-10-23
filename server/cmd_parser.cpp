#include "cmd_parser.hpp"
#include "msglog.hpp"
#include "server_messages.hpp"

llog::MessagePtr llog::parse_cmd(const std::string &cmd) {

    if (cmd == "q" || cmd == "exit" || cmd == "quit")
        return ServerShutdownMessage::create();

    return GenericMessage::create(severity::WARNING, "llog-server", "Unknown command '"+cmd+"'");
}
