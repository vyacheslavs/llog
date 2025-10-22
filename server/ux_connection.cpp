#include "ux_connection.hpp"
#include "server_messages.hpp"
#include "client_messages.hpp"

#include <unistd.h>

namespace {
    constexpr auto CON_BUF_SIZE = 1024;
    constexpr auto MAX_MSG_LEN = 1024*1024;
}

int llog::UxConnection::fd() const {
    return m_fd;
}

llog::UxConnection::~UxConnection() {
    if (m_fd >= 0) {
        close(m_fd);
    }
}

void llog::UxConnection::read(HandlerChainLinkPtr h) {

    auto [ptr, sz] = m_connection_buf.allocate(CON_BUF_SIZE);
    auto rd =::read(m_fd, ptr, sz);

    if (rd <= 0) {
        m_log->log(severity::WARNING, "Connection closed");
        process_chain(h, ServerConnectionDeadMessage::create(m_fd));
        return;
    }
    m_connection_buf.commit(rd);

    constexpr auto message_type_hdr_len = 2;
    constexpr auto message_len_hdr_len = 2;

    while (true) {
        if (m_proto_state == proto_state::INIT) {

            if (m_connection_buf.available(message_type_hdr_len)) {
                auto* _ptr = reinterpret_cast<uint16_t*> (m_connection_buf.pop_data(message_type_hdr_len));

                // acceptable message types are: LOG_MSG_TYPE_CLIENT_CONNECT
                if (static_cast<MessageType>(*_ptr) == MessageType::LOG_MSG_TYPE_CLIENT_CONNECT) {
                    m_msg_type = MessageType::LOG_MSG_TYPE_CLIENT_CONNECT;
                    m_proto_state = proto_state::READ_MSG_TYPE_DONE;
                } else {
                    // not acceptable, close the connection
                    m_log->log(severity::WARNING, "Connection closed due to invalid message type");
                    process_chain(h, ServerConnectionDeadMessage::create(m_fd));
                    return;
                }
            } else
                break;
        }
        if (m_proto_state == proto_state::READ_MSG_TYPE_DONE) {

            if (m_connection_buf.available(message_len_hdr_len)) {
                auto* _ptr = reinterpret_cast<uint16_t*> (m_connection_buf.pop_data(message_len_hdr_len));
                m_msg_len = *_ptr;
                if (m_msg_len < message_type_hdr_len + message_len_hdr_len) {
                    m_log->log(severity::WARNING, "Connection closed due to invalid message length");
                    process_chain(h, ServerConnectionDeadMessage::create(m_fd));
                    return;
                }
                m_proto_state = proto_state::READ_MSG_LEN_DONE;
            } else
                break;
        }
        if (m_proto_state == proto_state::READ_MSG_LEN_DONE) {
            auto data_size_needed = m_msg_len - message_type_hdr_len - message_len_hdr_len;
            if (m_connection_buf.available(data_size_needed)) {
                process_chain(h, parse_client_connect_message(m_msg_type, m_connection_buf.pop_data(data_size_needed), data_size_needed, m_fd));
                m_proto_state = proto_state::INIT;
            } else
                break;
        }
    }

}

bool llog::UxConnection::handle(MessagePtr msg) {
    if (msg->type() == MessageType::LOG_MSG_TYPE_CLIENT_CONNECT) {
        auto msg_cast = std::dynamic_pointer_cast<ClientConnectMessage>(msg);
        if (msg_cast && msg_cast->fd() == m_fd) {
            m_log->log(severity::INFO, "Client connected: " + msg_cast->id());
            m_id = msg_cast->id();
            return true;
        }
    }
    return false;
}
