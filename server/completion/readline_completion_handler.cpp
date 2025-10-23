#include "readline_completion_handler.hpp"

#include <cstring>

char * llog::generator_generic(const char *text, int state, const char **values, int &list_index, int &len) {
    const char *name;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }

    while ((name = values[list_index++])) {
        if (strncmp(name, text, len) == 0) {
            return strdup(name);
        }
    }
    return nullptr;
}

bool llog::any_of(const std::string &cmd, const char **variants) {
    for (auto* p = variants; *p; ++p) {
        if (*p == cmd)
            return true;
    }
    return false;
}

bool llog::any_of_starts_with(const std::string &cmd, const char **variants) {
    for (auto* p = variants; *p; ++p) {
        std::string sub = *p;
        if (cmd.size() < sub.size() && sub.compare(0, cmd.size(), cmd) == 0) {
            return true;
        }
    }
    return false;
}
