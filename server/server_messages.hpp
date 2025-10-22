#pragma once

#include "message.hpp"

namespace llog {
    using ServerShutdownMessagePtr = std::shared_ptr<class ServerShutdownMessage>;
    class ServerShutdownMessage : public Message {
        public:
            [[nodiscard]] MessageType type() const override {
                return MessageType::LOG_MSG_TYPE_SERVER_SHUTDOWN;
            }

            static ServerShutdownMessagePtr create() {
                return ServerShutdownMessagePtr(new ServerShutdownMessage());
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
    };
}
