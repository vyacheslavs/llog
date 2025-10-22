#include "ux_connection.hpp"
#include "server_messages.hpp"

#include <unistd.h>

namespace {
    constexpr auto CON_BUF_SIZE = 1024;
}

int llog::UxConnection::fd() const {
    return m_fd;
}

llog::UxConnection::~UxConnection() {
    if (m_fd >= 0) {
        close(m_fd);
    }
}

llog::MessagePtr llog::UxConnection::read() {

    auto [ptr, sz] = m_connection_buf.allocate(CON_BUF_SIZE);
    auto rd =::read(m_fd, ptr, sz);

    if (rd == 0) {
        return ServerConnectionDeadMessage::create(m_fd);
    }

    return {};
}

bool llog::UxConnection::handle(MessagePtr msg) {
    return false;
}
