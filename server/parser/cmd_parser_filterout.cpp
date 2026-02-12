#include "cmd_parser_filterout.hpp"

llog::CmdParserFilterOutPtr llog::CmdParserFilterOut::create(const std::string &filter_text) {
    CmdParserFilterOutPtr r(new CmdParserFilterOut);
    r->m_filter_text = filter_text;
    return std::move(r);
}

llog::MessageType llog::CmdParserFilterOut::type() const {
    return MessageType::LOG_MSG_TYPE_SERVER_FILTEROUT_CHANGE;
}

const std::string & llog::CmdParserFilterOut::filter_text() const {
    return m_filter_text;
}

llog::MessagePtr llog::CmdParserFilterOutParser::parse(const std::vector<std::string> &msg) {
    if (msg.size() == 2 && msg[0] == "filterout")
        return CmdParserFilterOut::create(msg[1]);
    else if (msg.size() == 1 && msg[0] == "filterout")
        return CmdParserFilterOut::create(std::string());
    return {};
}

