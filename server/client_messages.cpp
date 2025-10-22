#include "client_messages.hpp"

#include <iostream>

#include "msglog.hpp"

llog::ClientConnectMessagePtr llog::ClientConnectMessage::create(const uint8_t *payload, size_t size, int fd) {
    ClientConnectMessagePtr r(new ClientConnectMessage);

    r->m_id = std::string(payload, payload + size);
    r->m_fd = fd;

    return r;
}

std::string llog::ClientConnectMessage::id() const {
    return m_id;
}

int llog::ClientConnectMessage::fd() const {
    return m_fd;
}

llog::MessagePtr llog::parse_client_connect_message(MessageType mt, const uint8_t *payload, size_t size, int fd, const std::string& id) {

    if (mt == MessageType::LOG_MSG_TYPE_CLIENT_CONNECT)
        return ClientConnectMessage::create(payload, size, fd);
    else if (mt == MessageType::LOG_MSG_GENERIC)
        return GenericMessage::create(id, payload, size);
    return {};
}
