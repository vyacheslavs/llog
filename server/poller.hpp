#pragma once
#include <chrono>
#include <map>
#include <memory>
#include <vector>
#include <sys/select.h>
#include "descriptor.hpp"

namespace llog {

    using PollerPtr = std::shared_ptr<class Poller>;

    class Poller {
        public:

            enum class PollType : int {
                READ = 1,
                WRITE = 2,
                READ_WRITE = READ | WRITE,
            };

            static PollerPtr create();
            void add(DescriptorUsablePtr descriptor, PollType pt);
            void remove(DescriptorUsablePtr descriptor);
            bool poll(std::chrono::milliseconds timeout);
            bool has_events(DescriptorUsablePtr desc) const;

        private:
            Poller() = default;

            fd_set m_read_fds;
            fd_set m_write_fds;
            int m_max_fd {-1};

            struct client_info {
                DescriptorUsablePtr descriptor;
                PollType pt;
            };

            std::map<int, client_info> m_clients;

    };

}

