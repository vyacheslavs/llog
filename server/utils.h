#pragma once

#include <string>
#include <vector>

namespace llog::utils {
    std::vector<std::string> splitString(const std::string& inputString, char delimiter);
    std::string create_session_path(const std::string& session_hint);
}

