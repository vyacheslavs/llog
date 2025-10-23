#pragma once
#include <vector>

#include "message.hpp"

namespace llog {

    struct parser {
        virtual ~parser() = default;
        virtual MessagePtr parse(const std::vector<std::string>& msg) = 0;
    };

}
