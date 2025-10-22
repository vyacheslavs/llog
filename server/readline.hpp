#pragma once

#include "handler_chain.hpp"
#include "descriptor.hpp"

namespace llog {
    using ReadlinePtr = std::shared_ptr<class Readline>;
    class Readline : public HandlerChainLink, public DescriptorUsable {
        public:
            static ReadlinePtr create();
            bool handle(MessagePtr msg) override;

            [[nodiscard]] int fd() const override;
            void read();

        private:
            Readline() = default;
    };
}
