#include "msglog.hpp"

#include <iomanip>
#include <sstream>


llog::GenericMessagePtr llog::GenericMessage::create(severity sev, const std::string& id, const std::string &msg) {
    GenericMessagePtr r(new GenericMessage);

    r->m_severity = sev;
    r->m_time = std::chrono::system_clock::now();
    r->m_msg = msg;
    r->m_id = id;

    return std::move(r);
}

llog::MessageType llog::GenericMessage::type() const {
    return MessageType::LOG_MSG_GENERIC;
}

std::string llog::GenericMessage::formatted() const {
    std::stringstream ss;

    auto in_time_t = std::chrono::system_clock::to_time_t(m_time);

    static const std::vector<std::string> sev_names = {
        "\e[0;31mERROR  \e[0;37m",
        "\e[0;33mWARNING\e[0;37m",
        "\e[1;37mINFO   \e[0;37m",
        "VERBOSE",
        "DEBUG  ",
        "TRACE  ",
    };

    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(m_time.time_since_epoch()) % 1000;

    if (static_cast<int>(m_severity) >= 0 && static_cast<int>(m_severity) < sev_names.size())
        ss << "[" << sev_names[static_cast<int>(m_severity)] << "]";
    ss << "[" << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";

    ss << "\e[1;37m[" << m_id << "\e[0;37m] ";

    if (m_severity == severity::ERROR)
        ss << "\e[0;31m";
    else if (m_severity == severity::WARNING)
        ss << "\e[0;33m";
    else if (m_severity == severity::INFO)
        ss << "\e[1;37m";

    ss << m_msg << "\e[0;37m";

    return ss.str();
}

llog::MsgLogPtr llog::MsgLog::create(HandlerChainLinkPtr h, const std::string& id) {
    MsgLogPtr r(new MsgLog);
    r->m_handler = std::move(h);
    r->m_id = id;
    return std::move(r);
}

bool llog::MsgLog::log(severity severity, const std::string &message) {
    auto msg = GenericMessage::create(severity, m_id, message);
    process_chain(m_handler, msg);
    return true;
}
