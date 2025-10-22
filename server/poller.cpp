#include "poller.hpp"

#include <iostream>
#include <ostream>
#include <stdexcept>

#include "server_messages.hpp"

llog::PollerPtr llog::Poller::create() {
    auto p = PollerPtr(new Poller);

    return std::move(p);
}

void llog::Poller::add(DescriptorUsablePtr descriptor, PollType pt) {
    m_clients[descriptor->fd()] = {descriptor, pt, add_next(descriptor)};
}

void llog::Poller::remove(DescriptorUsablePtr descriptor) {
    remove_by_fd(descriptor->fd());
}

void llog::Poller::remove_by_fd(int fd) {
    remove_next(m_clients[fd].h_it);
    m_clients.erase(fd);
}

void llog::Poller::mark_dead(int fd) {
    m_clients[fd].descriptor->mark_dead();
}

bool llog::Poller::poll(std::chrono::milliseconds timeout) {
    timeval tv;
    tv.tv_sec = timeout.count() / 1000;
    tv.tv_usec = (timeout.count() % 1000) * 1000;

    FD_ZERO(&m_read_fds);
    FD_ZERO(&m_write_fds);

    m_max_fd = -1;
    for (auto client_it = m_clients.begin(); client_it != m_clients.end(); ++client_it) {
        if (!client_it->second.descriptor->alive()) {
            remove_next(m_clients[client_it->first].h_it); // keep in sync with remove_by_fd
            client_it = m_clients.erase(client_it);
        } else {
            auto& client = *client_it;
            if (client.second.pt == PollType::READ) {
                FD_SET(client.first, &m_read_fds);
            } else if (client.second.pt == PollType::WRITE) {
                FD_SET(client.first, &m_write_fds);
            }
            m_max_fd = std::max(m_max_fd, client.first);
        }
    }

    auto res = select(m_max_fd + 1, &m_read_fds, &m_write_fds, nullptr, &tv);

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

    if (msg->type() == MessageType::LOG_MSG_TYPE_SERVER_CONNECTION_DEAD) {
        auto msg_cast = std::dynamic_pointer_cast<ServerConnectionDeadMessage>(msg);
        if (msg_cast) {
            // don't remove immediately, just mark is as dead
            mark_dead(msg_cast->fd());
            return true;
        }
    }

    return false;
}


