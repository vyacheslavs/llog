#pragma once

#include "server/log.hpp"

/**
 * CS protocol:
 *
 * [message_type 2bytes][message_len 2bytes][message_payload]
 *
 * message_type: 1 (client connect)
 *   payload is client id.
 *
 * message_type: 2 (publish log)
 *   payload is: [severity 2bytes] [timestamp 8b][message - rest of payload]
 *
 */
namespace llog::client {

    class Log : public llog::Log {
        public:
            void log(llog::severity s, const std::string &message) override;
            static LogPtr create(const std::string& sock_path, const std::string& id);
        private:
            Log() = default;
    };

    using LogPtr = std::shared_ptr<class Log>;

}

