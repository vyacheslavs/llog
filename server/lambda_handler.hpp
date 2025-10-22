#pragma once

#include <functional>

#include "handler_chain.hpp"

namespace llog {

    using LambdaHandlerLinkPtr = std::shared_ptr<class LambdaHandlerLink>;

    class LambdaHandlerLink : public HandlerChainLink {
        public:
            static LambdaHandlerLinkPtr create(std::function<bool(MessagePtr)> handler) {
                LambdaHandlerLinkPtr link = std::shared_ptr<LambdaHandlerLink>(new LambdaHandlerLink());
                link->m_handler = std::move(handler);
                return link;
            }

            bool handle(MessagePtr msg) override {
                return m_handler(msg);
            }

        private:
            LambdaHandlerLink() = default;
            std::function<bool(MessagePtr)> m_handler;
    };

}
