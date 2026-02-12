#pragma once

#include "cmd_parser_common.hpp"
#include "log.hpp"

namespace llog {
    using CmdParserFilterOutPtr = std::shared_ptr<class CmdParserFilterOut>;
    class CmdParserFilterOut : public Message {
        public:
            static CmdParserFilterOutPtr create(const std::string& filter_text);
            [[nodiscard]] MessageType type() const override;
            [[nodiscard]] const std::string& filter_text() const;

        private:
            CmdParserFilterOut() = default;
            std::string m_filter_text;
    };

    struct CmdParserFilterOutParser : public parser {
        MessagePtr parse(const std::vector<std::string>& msg) override;
    };
}