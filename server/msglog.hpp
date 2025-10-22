#pragma once

#include <chrono>

#include "log.hpp"
#include "handler_chain.hpp"
#include "message.hpp"

namespace llog {

    using GenericMessagePtr = std::shared_ptr<class GenericMessage>;
    class GenericMessage : public Message {
        public:
            static GenericMessagePtr create(severity sev, const std::string& id, const std::string& msg);

            [[nodiscard]] MessageType type() const override;
            [[nodiscard]] std::string formatted() const;

        private:
            GenericMessage() = default;

            std::chrono::system_clock::time_point m_time;
            std::string m_msg;
            severity m_severity {severity::DEBUG};
            std::string m_id;
    };

    using MsgLogPtr = std::shared_ptr<class MsgLog>;
    class MsgLog : public Log {
    public:
        static MsgLogPtr create(HandlerChainLinkPtr h, const std::string& id);

        bool log(severity, const std::string &message) override;

    private:
        MsgLog() = default;
        HandlerChainLinkPtr m_handler;
        std::string m_id;
    };
}
