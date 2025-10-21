#include "handler_chain.hpp"

void llog::HandlerChainLink::push_next(HandlerChain &queue) {
    std::copy(m_next.begin(), m_next.end(), std::back_inserter(queue));
}

void llog::HandlerChainLink::add_next(HandlerChainLinkPtr link) {
    m_next.push_back(link);
}

void llog::process_chain(HandlerChainLinkPtr &first, MessagePtr msg) {
    HandlerChain queue;
    queue.push_back(first);

    for (auto h_it = queue.begin(); h_it != queue.end(); ++h_it) {
        if ((*h_it)->handle(msg)) // if message been handled, then stop queue chain
            return;
        (*h_it)->push_next(queue);
    }
}
