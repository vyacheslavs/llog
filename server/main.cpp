
#include <iostream>
#include <list>
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

    std::list<llog::UxConnectionPtr> clients;

    while (true) {
        if (!poller->poll(std::chrono::milliseconds(1000)))
            break;

        if (poller->has_events(server)) {
            logger->log(llog::severity::INFO, "server has events");

            auto new_conn = server->accept();
            if (new_conn) {
                poller->add(new_conn, llog::Poller::PollType::READ);
                clients.push_back(std::move(new_conn));
            }
        }

        for (auto &client : clients) {
            if (poller->has_events(client)) {
                logger->log(llog::severity::INFO, "client has events");
            }
        }
    }

    return 0;
}
