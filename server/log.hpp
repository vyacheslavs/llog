#pragma once
#include <memory>
#include <string>

namespace llog {
    class Log {
        public:
            virtual void log(const std::string& message);
            virtual ~Log() = default;
    };

    using LogPtr = std::shared_ptr<class Log>;
}
