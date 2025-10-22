#include "handler_chain.hpp"

void llog::HandlerChainLink::push_next(HandlerChain &queue) {
    std::copy(m_next.begin(), m_next.end(), std::back_inserter(queue));
}

void llog::HandlerChainLink::push_message(MessageList& msg) {
    std::copy(m_messages.begin(), m_messages.end(), std::back_inserter(msg));
}

void llog::HandlerChainLink::add_message(MessagePtr msg) {
    m_messages.push_back(msg);
}

llog::HandlerChain::iterator llog::HandlerChainLink::add_next(HandlerChainLinkPtr link) {
    m_next.push_back(link);
    auto it = m_next.end();
    return --it;
}

void llog::HandlerChainLink::remove_next(HandlerChain::iterator it) {
    m_next.erase(it);
}

void llog::process_chain(HandlerChainLinkPtr &first, MessagePtr msg) {

    MessageList messages;
    messages.push_back(msg);

    for (auto m_it = messages.begin(); m_it != messages.end(); ++m_it) {
        HandlerChain queue;
        queue.push_back(first);

        for (auto h_it = queue.begin(); h_it != queue.end(); ++h_it) {
            if ((*h_it)->handle(*m_it)) // if message been handled, then stop queue chain
                return;
            (*h_it)->push_next(queue);
            (*h_it)->push_message(messages);
        }
    }
}
