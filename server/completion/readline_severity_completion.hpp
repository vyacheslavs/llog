#pragma once

#include "readline_completion_handler.hpp"

namespace llog {

    struct severity_completion : completion_handler {
        completion_generator can_complete(const std::vector<std::string>& cmd) override;
    };

}