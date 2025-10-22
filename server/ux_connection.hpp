#pragma once

#include "descriptor.hpp"
#include "message.hpp"
#include <memory>
#include "dqueue.hpp"

namespace llog {

    using UxConnectionPtr = std::shared_ptr<class UxConnection>;

    class UxConnection : public DescriptorUsable {
        public:

            [[nodiscard]] int fd() const override;
            ~UxConnection() override;

            MessagePtr read();

            bool handle(MessagePtr msg) override;

        private:
            UxConnection() = default;
            friend class UxServer;

            int m_fd {-1};

            DQueue m_connection_buf;
    };

}