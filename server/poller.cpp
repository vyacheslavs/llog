#include "poller.hpp"

#include <stdexcept>

llog::PollerPtr llog::Poller::create() {
    auto p = PollerPtr(new Poller);

    return std::move(p);
}

void llog::Poller::add(DescriptorUsablePtr descriptor, PollType pt) {
    m_clients[descriptor->fd()] = {descriptor, pt};

}

void llog::Poller::remove(DescriptorUsablePtr descriptor) {
    m_clients.erase(descriptor->fd());
}

bool llog::Poller::poll(std::chrono::milliseconds timeout) {
    timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    FD_ZERO(&m_read_fds);
    FD_ZERO(&m_write_fds);

    m_max_fd = -1;
    for (auto &client : m_clients) {
        if (client.second.pt == PollType::READ) {
            FD_SET(client.first, &m_read_fds);
        } else if (client.second.pt == PollType::WRITE) {
            FD_SET(client.first, &m_write_fds);
        }
        m_max_fd = std::max(m_max_fd, client.first);
    }

    auto res = select(m_max_fd, &m_read_fds, &m_write_fds, nullptr, &tv);
    return res >= 0;
}

bool llog::Poller::has_events(DescriptorUsablePtr desc) const {
    return FD_ISSET(desc->fd(), &m_read_fds) || FD_ISSET(desc->fd(), &m_write_fds);
}

llog::Poller::iterator llog::Poller::begin() {
    return m_clients.begin();
}

llog::Poller::iterator llog::Poller::end() {
    return m_clients.end();
}

llog::Poller::iterator llog::Poller::erase(iterator it) {
    return m_clients.erase(it);
}

bool llog::Poller::handle(MessagePtr msg) {
    return false;
}


