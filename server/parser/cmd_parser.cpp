#include "cmd_parser.hpp"

#include "cmd_parser_common.hpp"
#include "cmd_parser_severity.hpp"
#include "msglog.hpp"
#include "server_messages.hpp"
#include "utils.h"

llog::MessagePtr llog::parse_cmd(const std::string &cmd) {

    static std::unique_ptr<parser> parsers[] = {
        std::make_unique<SeverityParser>()
    };

    if (cmd == "q" || cmd == "exit" || cmd == "quit")
        return ServerShutdownMessage::create();

    auto cmd_v = llog::utils::splitString(cmd, ' ');

    for (auto& parser : parsers)
        if (auto msg = parser->parse(cmd_v))
            return msg;

    return GenericMessage::create(severity::WARNING, "llog-server", "Unknown command '"+cmd+"'");
}
