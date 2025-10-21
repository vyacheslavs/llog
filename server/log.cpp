#include "log.hpp"

#include <iostream>
#include <ostream>

bool llog::Log::log(severity s, const std::string &message) {
    std::cout << message << std::endl;
    return true;
}
