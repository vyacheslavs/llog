#pragma once
#include <memory>
#include "log.hpp"
#include "descriptor.hpp"
#include "ux_connection.hpp"

namespace llog {

    using UxServerPtr = std::shared_ptr<class UxServer>;

    class UxServer : public DescriptorUsable {
        public:
            UxServer(const UxServer&) = delete;
            UxServer& operator=(const UxServer&) = delete;
            UxServer(UxServer&&) = delete;
            UxServer& operator=(UxServer&&) = delete;

            static UxServerPtr create(LogPtr logger, const std::string& sock_path);
            [[nodiscard]] int fd() const override;
            [[nodiscard]] UxConnectionPtr accept() const;

            bool handle(MessagePtr msg) override;

            void set_logger(LogPtr logger);

            ~UxServer() override;
        private:
            UxServer() = default;

            int m_fd {-1};
            LogPtr m_logger;
    };

}
