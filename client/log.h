#pragma once

#include <vector>

#include "server/log.hpp"

/**
 * CS protocol:
 *
 * [message_type 2bytes][message_len 2bytes(includes all)][message_payload]
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
            bool log(llog::severity s, const std::string &message) override;
            static LogPtr create(const std::string& sock_path, const std::string& id);

            ~Log();
        private:

            struct introduction_pkg {
                __uint16_t type;
                __uint16_t len;
                __uint8_t id;
            } __attribute__((packed));

            struct generic_msg_pkg {
                __uint16_t type;
                __uint16_t len;
                __uint16_t severity;
                __uint64_t timestamp;
                __uint8_t message;
            } __attribute__((packed));

            Log() = default;

            int m_fd {-1};
            std::string m_id;
            bool m_client_introduced {false};
            std::vector<uint8_t> m_client_buffer;
    };

    using LogPtr = std::shared_ptr<class Log>;

}

