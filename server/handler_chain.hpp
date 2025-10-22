#pragma once
#include <list>
#include <memory>
#include <vector>

#include "message.hpp"

namespace llog {

    using HandlerChainLinkPtr = std::shared_ptr<class HandlerChainLink>;
    using HandlerChain = std::list<HandlerChainLinkPtr>;

    class HandlerChainLink {
        public:
            virtual ~HandlerChainLink() = default;

            virtual void push_next(HandlerChain& queue);
            virtual void push_message(MessageList& msg);

            virtual bool handle(MessagePtr msg) = 0;

            void add_next(HandlerChainLinkPtr link);
            void add_message(MessagePtr msg);

        private:

            std::vector<HandlerChainLinkPtr> m_next;
            std::vector<MessagePtr> m_messages;
    };

    void process_chain(HandlerChainLinkPtr& first, MessagePtr msg);

}
