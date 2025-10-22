
#include <iostream>
#include <list>
#include <memory>

#include "utils.h"
#include "ux_server.hpp"
#include "client/log.h"
#include "poller.hpp"
#include "lambda_handler.hpp"
#include "readline.hpp"
#include "msglog.hpp"

llog::HandlerChainLinkPtr create_handler_graph(bool* running_ptr) {

    llog::HandlerChainLinkPtr chain_head = llog::LambdaHandlerLink::create([running_ptr](llog::MessagePtr msg) {
        if (msg->type() == llog::MessageType::LOG_MSG_TYPE_SERVER_SHUTDOWN) {
            *running_ptr = false;
            return true;
        }
        return false;
    });

    return chain_head;
}

int main(int argc, char **argv) {

    auto session_path = create_session_path({});
    if (argc > 1) {
        session_path = create_session_path(argv[1]);
    }

    bool running = true;
    auto server_handler_chain = create_handler_graph(&running);

    auto rl = llog::Readline::create(server_handler_chain);
    if (!rl)
        return 3;

    auto logger = llog::MsgLog::create(rl);
    logger->log(llog::severity::INFO,"starting the llog daemon with session path: " + session_path);
    auto server = llog::UxServer::create(logger, session_path);

    if (!server)
        return 1;

    auto poller = llog::Poller::create();
    if (!poller)
        return 2;

    // server_handler -> poller
    server_handler_chain->add_next(poller);

    poller->add(server, llog::Poller::PollType::READ);
    poller->add(rl, llog::Poller::PollType::READ);

    while (running) {
        if (!poller->poll(std::chrono::milliseconds(1000)))
            break;

        if (poller->has_events(server)) {
            logger->log(llog::severity::DEBUG, "server has a new connection");

            if (auto new_conn = server->accept()) {
                poller->add(new_conn, llog::Poller::PollType::READ);
            } else {
                logger->log(llog::severity::ERROR, "failed to accept connection");
                break;
            }
        }

        if (poller->has_events(rl)) {
            rl->read();
        }

        for (auto& [client_fd, client] : *poller) {
            if (auto ux_connection = dynamic_cast<llog::UxConnection*>(client.descriptor.get())) {
                logger->log(llog::severity::INFO, "client has events");

                llog::process_chain(server_handler_chain, ux_connection->read());
            }
        }
    }

    return 0;
}
