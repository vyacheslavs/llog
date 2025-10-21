#pragma once

#include "descriptor.hpp"
#include <memory>

namespace llog {

    using UxConnectionPtr = std::shared_ptr<class UxConnection>;

    class UxConnection : public DescriptorUsable {
        public:

            [[nodiscard]] int fd() const override;
            ~UxConnection() override;

    private:
            UxConnection() = default;
            friend class UxServer;

            int m_fd {-1};
    };

}