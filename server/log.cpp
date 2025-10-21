#include "log.hpp"

#include <iostream>
#include <ostream>

void llog::Log::log(severity s, const std::string &message) {
    std::cout << message << std::endl;
}
