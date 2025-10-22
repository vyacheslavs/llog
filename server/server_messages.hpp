#pragma once

#include "message.hpp"

namespace llog {
    class ServerShutdownMessage : public Message {
        public:
            [[nodiscard]] MessageType type() const override {
                return MessageType::LOG_MSG_TYPE_SERVER_SHUTDOWN;
            }
        private:
            ServerShutdownMessage() = default;
    };

    class ServerConnectionDeadMessage : public Message {
        public:
            [[nodiscard]] MessageType type() const override {
                return MessageType::LOG_MSG_TYPE_SERVER_CONNECTION_DEAD;
            }
        private:
            ServerConnectionDeadMessage() = default;
    }
}
