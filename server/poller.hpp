#pragma once
#include <chrono>
#include <map>
#include <memory>
#include <vector>
#include <sys/select.h>
#include "descriptor.hpp"
#include "handler_chain.hpp"

namespace llog {

    using PollerPtr = std::shared_ptr<class Poller>;

    class Poller : public HandlerChainLink {
        public:
            enum class PollType : int {
                READ = 1,
                WRITE = 2,
                READ_WRITE = READ | WRITE,
            };
            struct client_info {
                DescriptorUsablePtr descriptor;
                PollType pt;
                HandlerChain::iterator h_it;
            };

            static PollerPtr create();
            void add(DescriptorUsablePtr descriptor, PollType pt);
            void remove(DescriptorUsablePtr descriptor);
            void remove_by_fd(int fd);
            void mark_dead(int fd);
            bool poll(std::chrono::milliseconds timeout);
            [[nodiscard]] bool has_events(DescriptorUsablePtr desc) const;

            using iterator = std::map<int, client_info>::iterator;

            iterator begin();
            iterator end();
            iterator erase(iterator it);

            bool handle(MessagePtr msg) override;

        private:
            Poller() = default;

            fd_set m_read_fds;
            fd_set m_write_fds;
            int m_max_fd {-1};

            std::map<int, client_info> m_clients;

    };

}

