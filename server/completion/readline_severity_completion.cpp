#include "readline_severity_completion.hpp"

#include <iostream>
#include <string.h>

namespace {

    const char *severities[] = {
        "error",
        "warning",
        "info",
        "verbose",
        "debug",
        "trace",
        nullptr
    };

    const char* sev_values[] = {
        "on",
        "off",
        nullptr
    };

    char * severities_generator(const char *text, int state) {
        static int list_index, len;
        return llog::generator_generic(text, state, severities, list_index, len);
    }

    char* sev_values_generator(const char *text, int state) {
        static int list_index, len;
        return llog::generator_generic(text, state, sev_values, list_index, len);
    }

}

llog::completion_generator llog::severity_completion::can_complete(const std::vector<std::string> &cmd) {

    if (cmd.size() == 1 && cmd[0] == "severity")
        return severities_generator;

    if (cmd.size() == 2 && cmd[0] == "severity") {
        if (any_of_starts_with(cmd[1], severities))
            return severities_generator;
    }

    if (cmd.size() == 2 && cmd[0] == "severity" && any_of(cmd[1], severities))
        return sev_values_generator;

    if (cmd.size() == 3 && cmd[0] == "severity" && any_of(cmd[1], severities) && any_of_starts_with(cmd[2], sev_values))
        return sev_values_generator;

    return nullptr;
}

