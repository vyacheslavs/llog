#pragma once
#include <memory>
#include "log.hpp"

namespace llog {

    using UxServerPtr = std::shared_ptr<class UxServer>;
    using UxServerUPtr = std::unique_ptr<class UxServer>;

    class UxServer {
        public:
            UxServer(const UxServer&) = delete;
            UxServer& operator=(const UxServer&) = delete;
            UxServer(UxServer&&) = delete;
            UxServer& operator=(UxServer&&) = delete;

            static UxServerUPtr create(LogPtr logger, const std::string& sock_path);

            [[nodiscard]] int fd() const;

            void set_logger(LogPtr logger);

            ~UxServer();
        private:
            UxServer() = default;

            int m_fd {-1};
            LogPtr m_logger;
    };

}
