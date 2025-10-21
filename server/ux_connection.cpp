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
