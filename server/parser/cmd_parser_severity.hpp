#pragma once

#include "cmd_parser_common.hpp"
#include "log.hpp"

namespace llog {

    using SeverityOnOffPtr = std::shared_ptr<class SeverityOnOff>;
    class SeverityOnOff : public Message {
        public:
            static SeverityOnOffPtr create(severity sev, bool onoff);
            [[nodiscard]] MessageType type() const override;
            [[nodiscard]] std::pair<severity, bool> sev_change() const;

        private:
            SeverityOnOff() = default;
            severity m_sev{severity::INFO};
            bool m_onoff {true};
    };

    struct SeverityParser : public parser {
        MessagePtr parse(const std::vector<std::string>& msg) override;
    };
}
