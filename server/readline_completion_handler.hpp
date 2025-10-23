#pragma once
#include <string>
#include <vector>

namespace llog {

    using completion_generator = char* (*)(const char *text, int state);

    struct completion_handler {
        virtual ~completion_handler() = default;
        [[nodiscard]] virtual completion_generator can_complete(const std::vector<std::string>& cmd) = 0;
    };


}
