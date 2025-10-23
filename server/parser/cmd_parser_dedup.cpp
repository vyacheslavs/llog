#include "cmd_parser_dedup.hpp"
#include <string>

llog::DedupOnOffPtr llog::DedupOnOff::create(DedupType onoff) {
    DedupOnOffPtr r(new DedupOnOff);
    r->m_onoff = onoff;
    return std::move(r);
}

llog::MessageType llog::DedupOnOff::type() const {
    return MessageType::LOG_MSG_TYPE_SERVER_DEDUP_CHANGE;
}

llog::DedupOnOff::DedupType llog::DedupOnOff::onoff() const {
    return m_onoff;
}

llog::MessagePtr llog::DedupOnOffParser::parse(const std::vector<std::string> &msg) {

    if (msg.size() == 2 && msg[0] == "dedup") {
        return DedupOnOff::create(msg[1] == "on" ? DedupOnOff::DedupType::ON : (msg[1] == "off" ? DedupOnOff::DedupType::OFF : DedupOnOff::DedupType::INFO));
    }

    return {};
}

