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

    using ServerConnectionDeadMessagePtr = std::shared_ptr<class ServerConnectionDeadMessage>;
    class ServerConnectionDeadMessage : public Message {
        public:
            [[nodiscard]] MessageType type() const override {
                return MessageType::LOG_MSG_TYPE_SERVER_CONNECTION_DEAD;
            }

            static ServerConnectionDeadMessagePtr create(int fd) {
                auto msg = ServerConnectionDeadMessagePtr(new ServerConnectionDeadMessage());
                msg->m_fd = fd;
                return msg;
            }

            [[nodiscard]] int fd() const {
                return m_fd;
            }

        private:
            ServerConnectionDeadMessage() = default;
            int m_fd {-1};
    };
}
