#pragma once
#include <memory>
#include <string>

namespace llog {

    enum class severity : int {
        ERROR = 0,
        INFO = 1,
        VERBOSE = 2,
        DEBUG = 3,
        TRACE = 4,
    };

    class Log {
        public:
            virtual void log(severity, const std::string& message);
            virtual ~Log() = default;
    };

    using LogPtr = std::shared_ptr<class Log>;
}
