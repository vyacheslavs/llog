#pragma once

#include "handler_chain.hpp"
#include "descriptor.hpp"
#include "parser/cmd_parser_dedup.hpp"

namespace llog {
    using ReadlinePtr = std::shared_ptr<class Readline>;
    class Readline : public DescriptorUsable {
        public:
            static ReadlinePtr create(HandlerChainLinkPtr handler_root);
            bool handle(MessagePtr msg) override;

            [[nodiscard]] int fd() const override;
            void read();
            ~Readline() override;

        private:
            Readline() = default;
            std::vector<bool> m_severity_settings;
            DedupOnOff::DedupType m_dedup {DedupOnOff::DedupType::ON};
    };
}
