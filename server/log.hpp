#pragma once
#include <memory>
#include <string>

namespace llog {

    enum class severity : int {
        ERROR   = 0,
        WARNING = 1,
        INFO    = 2,
        VERBOSE = 3,
        DEBUG   = 4,
        TRACE   = 5,
    };

    class Log {
        public:
            virtual bool log(severity, const std::string& message);
            virtual ~Log() = default;
    };

    using LogPtr = std::shared_ptr<class Log>;

}
