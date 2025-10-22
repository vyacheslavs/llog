#include "ux_server.hpp"

#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

llog::UxServerPtr llog::UxServer::create(LogPtr logger, const std::string& sock_path) {
    UxServerPtr server(new UxServer);
    server->m_logger = std::move(logger);

    if ((server->m_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        server->m_logger->log(severity::ERROR, "failed to create socket");
        return {};
    }

    unlink(sock_path.c_str());

    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path.c_str(), sizeof(addr.sun_path)-1);

    if (bind(server->m_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        server->m_logger->log(severity::ERROR, "failed to bind socket");
        return {};
    }

    if (listen(server->m_fd, 5) < 0) {
        server->m_logger->log(severity::ERROR, "failed to listen on socket");
        return {};
    }

    return server;
}

int llog::UxServer::fd() const {
    return m_fd;
}

llog::UxConnectionPtr llog::UxServer::accept() const {
    struct sockaddr_un addr;
    socklen_t addrlen = sizeof(addr);

    auto new_fd = ::accept(m_fd, (struct sockaddr *)&addr, &addrlen);
    if (new_fd < 0) {
        return {};
    }

    llog::UxConnectionPtr conn(new UxConnection);
    conn->m_fd = new_fd;

    return std::move(conn);
}

bool llog::UxServer::handle(MessagePtr msg) {
    return false;
}

void llog::UxServer::set_logger(LogPtr logger) {
    m_logger = std::move(logger);
}

llog::UxServer::~UxServer() {
    if (m_fd >= 0) {
        close(m_fd);
    }
}
