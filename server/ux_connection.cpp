#include "ux_connection.hpp"

#include <unistd.h>

int llog::UxConnection::fd() const {
    return m_fd;
}

llog::UxConnection::~UxConnection() {
    if (m_fd >= 0) {
        close(m_fd);
    }
}

bool llog::UxConnection::alive() const {
    return true;
}

llog::MessagePtr llog::UxConnection::read() {
    return {};
}

bool llog::UxConnection::handle(MessagePtr msg) {
    return false;
}
