#pragma once

namespace llog {

    using MessagePtr = std::shared_ptr<class Message>;

    enum class MessageType : int {
        LOG_MSG_TYPE_CLIENT_CONNECT         = 1,
        LOG_MSG_TYPE_SERVER_SHUTDOWN        = 5,
        LOG_MSG_TYPE_SERVER_CONNECTION_DEAD = 6,
    };

    class Message {
        public:
            virtual ~Message() = default;
            [[nodiscard]] virtual MessageType type() const = 0;

        private:
            Message() = default;
    };

}
