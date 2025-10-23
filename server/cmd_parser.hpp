#pragma once

#include <string>
#include "message.hpp"

namespace llog {

    MessagePtr parse_cmd(const std::string& cmd);

}
