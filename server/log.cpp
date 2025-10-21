#include "log.hpp"

#include <iostream>
#include <ostream>

void llog::Log::log(const std::string &message) {
    std::cout << message << std::endl;
}
