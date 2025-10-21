
#include <iostream>
#include <memory>

#include "utils.h"
#include "ux_server.hpp"
#include "client/log.h"

int main(int argc, char **argv) {

    auto session_path = create_session_path({});
    if (argc > 1) {
        session_path = create_session_path(argv[1]);
    }
    auto logger = std::make_shared<llog::Log>();
    logger->log(llog::severity::INFO,"starting the llog daemon with session path: " + session_path);
    auto server = llog::UxServer::create(logger, session_path);

    if (!server)
        return 1;

    auto client_log = llog::client::Log::create(session_path, "internal_logger");
    server->set_logger(client_log);



    return 0;
}
