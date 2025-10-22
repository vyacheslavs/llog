#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "message.hpp"

namespace llog {

    using ClientConnectMessagePtr = std::shared_ptr<class ClientConnectMessage>;
    class ClientConnectMessage : public Message {
        public:
            static ClientConnectMessagePtr create(const uint8_t* payload, size_t size, int fd);
            [[nodiscard]] MessageType type() const override {
                return MessageType::LOG_MSG_TYPE_CLIENT_CONNECT;
            }

            [[nodiscard]] std::string id() const;
            [[nodiscard]] int fd() const;

        private:
            ClientConnectMessage() = default;
            std::string m_id;
            int m_fd {-1};
    };

    MessagePtr parse_client_connect_message(MessageType mt, const uint8_t* payload, size_t size, int fd, const std::string& id);

}