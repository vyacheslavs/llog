#include "log.h"

void llog::client::Log::log(llog::severity s, const std::string &message) {

}

llog::LogPtr llog::client::Log::create(const std::string &sock_path, const std::string &id) {
    return {};
}
