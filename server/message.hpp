#pragma once
#include <list>

namespace llog {

    using MessagePtr = std::shared_ptr<class Message>;
    using MessageList = std::list<MessagePtr>;

    enum class MessageType : int {
        LOG_MSG_TYPE_CLIENT_CONNECT         = 1,
        LOG_MSG_TYPE_SERVER_SHUTDOWN        = 5,
        LOG_MSG_TYPE_SERVER_CONNECTION_DEAD = 6,
        LOG_MSG_READLINE_OUTPUT             = 7,
        LOG_MSG_GENERIC                     = 8,
    };

    class Message {
        public:
            virtual ~Message() = default;
            [[nodiscard]] virtual MessageType type() const = 0;
            Message() = default;

    };

}
