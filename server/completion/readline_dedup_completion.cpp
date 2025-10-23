#include "readline_dedup_completion.hpp"

namespace {
    const char* dedup_values[] = {
        "on",
        "off",
        nullptr
    };

    char * dedup_generator(const char *text, int state) {
        static int list_index, len;
        return llog::generator_generic(text, state, dedup_values, list_index, len);
    }

}

llog::completion_generator llog::dedup_completion::can_complete(const std::vector<std::string> &cmd) {

    if (cmd.size() == 1 && cmd[0] == "dedup")
        return dedup_generator;

    if (cmd.size() == 2 && cmd[0] == "dedup") {
        if (any_of_starts_with(cmd[1], dedup_values))
            return dedup_generator;
    }

    return nullptr;
}
