#pragma once

#include "descriptor.hpp"
#include "message.hpp"
#include <memory>

namespace llog {

    using UxConnectionPtr = std::shared_ptr<class UxConnection>;

    class UxConnection : public DescriptorUsable {
        public:

            [[nodiscard]] int fd() const override;
            ~UxConnection() override;

            [[nodiscard]] bool alive() const;
            MessagePtr read();

            bool handle(MessagePtr msg) override;

        private:
            UxConnection() = default;
            friend class UxServer;

            bool m_alive {true};
            int m_fd {-1};
    };

}