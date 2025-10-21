#include "log.h"

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

namespace {
    constexpr auto header_message_type_len = 2;
    constexpr auto header_message_len_len = 2;

    constexpr auto LOG_MSG_TYPE_CLIENT_CONNECT = 1;
}

bool llog::client::Log::log(llog::severity s, const std::string &message) {

    if (!m_client_introduced) {
        m_client_introduced = true;
        const auto introduction_pkg_len = m_id.size() + header_message_type_len + header_message_len_len;
        if (m_client_buffer.size() < introduction_pkg_len) m_client_buffer.resize(introduction_pkg_len);
        auto *pkg = reinterpret_cast<introduction_pkg*>(m_client_buffer.data());
        pkg->type = LOG_MSG_TYPE_CLIENT_CONNECT;
        pkg->len = introduction_pkg_len;
        std::memcpy(&pkg->id, m_id.data(), m_id.size());
        if (write(m_fd, m_client_buffer.data(), introduction_pkg_len) != introduction_pkg_len) {
            return false;
        }
    }

    return true;
}

llog::LogPtr llog::client::Log::create(const std::string &sock_path, const std::string &id) {

    if (sock_path.empty() || id.empty()) {
        return {};
    }

    // Ensure the provided socket path fits into sockaddr_un
    if (sock_path.size() >= sizeof(sockaddr_un{}.sun_path)) {
        return {};
    }

    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
        return {};
    }

    struct sockaddr_un remote;
    std::memset(&remote, 0, sizeof(remote));
    remote.sun_family = AF_UNIX;
    std::strncpy(remote.sun_path, sock_path.c_str(), sizeof(remote.sun_path) - 1);

    // addrlen should be up to the used sun_path bytes
    socklen_t addrlen = static_cast<socklen_t>(offsetof(struct sockaddr_un, sun_path) + std::strlen(remote.sun_path));

    if (connect(fd, reinterpret_cast<struct sockaddr *>(&remote), addrlen) == -1) {
        close(fd);
        return {};
    }

    LogPtr log(new Log);
    log->m_fd = fd;
    log->m_id = id;

    return log;
}

llog::client::Log::~Log() {
    if (m_fd >= 0) {
        close(m_fd);
    }
}
