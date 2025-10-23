#include "cmd_parser_severity.hpp"

#include <map>

llog::SeverityOnOffPtr llog::SeverityOnOff::create(severity sev, bool onoff) {
    SeverityOnOffPtr r(new SeverityOnOff);

    r->m_sev = sev;
    r->m_onoff = onoff;

    return std::move(r);
}

llog::MessageType llog::SeverityOnOff::type() const {
    return MessageType::LOG_MSG_TYPE_SERVER_SEVERITY_CHANGE;
}

std::pair<llog::severity, bool> llog::SeverityOnOff::sev_change() const {
    return {m_sev, m_onoff};
}

llog::MessagePtr llog::SeverityParser::parse(const std::vector<std::string> &msg) {

    static std::map<std::string, severity> sevmap = {
        {"error", severity::ERROR},
        {"warning", severity::WARNING},
        {"info", severity::INFO},
        {"verbose", severity::VERBOSE},
        {"debug", severity::DEBUG},
        {"trace", severity::TRACE},
    };

    if (msg.size() == 3 && msg[0] == "severity") {
        auto it = sevmap.find(msg[1]);
        if (it != sevmap.end())
            return SeverityOnOff::create(it->second, msg[2] == "on");
    }
    return {};
}
