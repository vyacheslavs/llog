#pragma once
#include <string>
#include <vector>

namespace llog {

    using completion_generator = char* (*)(const char *text, int state);

    struct completion_handler {
        virtual ~completion_handler() = default;
        [[nodiscard]] virtual completion_generator can_complete(const std::vector<std::string>& cmd) = 0;
    };

    char* generator_generic(const char *text, int state, const char** values, int& list_index, int& len);
    bool any_of(const std::string& cmd, const char ** variants);
    bool any_of_starts_with(const std::string& cmd, const char ** variants);

}
