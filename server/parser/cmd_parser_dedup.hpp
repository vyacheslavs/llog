#pragma once
#include "cmd_parser_common.hpp"
#include "message.hpp"

namespace llog {

    using DedupOnOffPtr = std::shared_ptr<class DedupOnOff>;
    class DedupOnOff : public Message {
        public:
            enum class DedupType {
                ON,
                OFF,
                INFO
            };

            static DedupOnOffPtr create(DedupType onoff);
            [[nodiscard]] MessageType type() const override;
            [[nodiscard]] DedupType onoff() const;
        private:
            DedupType m_onoff {DedupType::OFF};
    };

    struct DedupOnOffParser : public parser {
        MessagePtr parse(const std::vector<std::string>& msg) override;
    };

}
