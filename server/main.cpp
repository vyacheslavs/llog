
#include <iostream>
#include <memory>

#include "utils.h"
#include "ux_server.hpp"
#include "client/log.h"
#include "poller.hpp"

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

    // auto client_log = llog::client::Log::create(session_path, "internal_logger");
    // server->set_logger(client_log);

    auto poller = llog::Poller::create();
    if (!poller)
        return 2;

    poller->add(server, llog::Poller::PollType::READ);

    while (true) {
        poller->poll(std::chrono::milliseconds(1000));

        if (poller->has_events(server)) {
            logger->log(llog::severity::INFO, "server has events");
        }
    }

    return 0;
}
