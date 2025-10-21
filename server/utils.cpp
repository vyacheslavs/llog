#include "utils.h"

#include <iostream>

std::string create_session_path(const std::string &session_hint) {
    auto hint = session_hint;
    if (hint.empty()) {
        hint = "session_XXXXXX";
        mktemp(hint.data());
    }

    hint = "/tmp/" + hint;

    return hint;
}
