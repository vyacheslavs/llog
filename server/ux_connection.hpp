#pragma once

#include "descriptor.hpp"
#include "message.hpp"
#include <memory>
#include "dqueue.hpp"
#include "log.hpp"
#include "handler_chain.hpp"

namespace llog {

    using UxConnectionPtr = std::shared_ptr<class UxConnection>;

    class UxConnection : public DescriptorUsable {
        public:

            [[nodiscard]] int fd() const override;
            ~UxConnection() override;

            void read(HandlerChainLinkPtr chain);

            bool handle(MessagePtr msg) override;

        private:

            enum class proto_state {
                INIT,
                READ_MSG_TYPE_DONE,
                READ_MSG_LEN_DONE,
            };

            UxConnection() = default;
            friend class UxServer;

            int m_fd {-1};
            std::string m_id;

            DQueue m_connection_buf;
            proto_state m_proto_state {proto_state::INIT};
            MessageType m_msg_type {MessageType::LOG_MSG_TYPE_CLIENT_CONNECT};
            LogPtr m_log;

            uint16_t m_msg_len {0};
    };

}