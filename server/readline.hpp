#pragma once

#include "handler_chain.hpp"
#include "descriptor.hpp"

namespace llog {
    using ReadlinePtr = std::shared_ptr<class Readline>;
    class Readline : public HandlerChainLink, public DescriptorUsable {
        public:
            static ReadlinePtr create(HandlerChainLinkPtr handler_root);
            bool handle(MessagePtr msg) override;

            [[nodiscard]] int fd() const override;
            void read();
            ~Readline() override;

        private:
            Readline() = default;
    };
}
