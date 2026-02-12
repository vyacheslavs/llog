#include "readline.hpp"

#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "server_messages.hpp"

#include "log.hpp"
#include "msglog.hpp"
#include "completion/readline_completion_commands_gen.hpp"
#include "parser/cmd_parser.hpp"
#include "parser/cmd_parser_severity.hpp"
#include "parser/cmd_parser_dedup.hpp"
#include "parser/cmd_parser_filterout.hpp"

namespace {

    constexpr auto PROMPT = "llog> ";
    llog::HandlerChainLinkPtr handler_root;

    void line_handler(char *line) {
        llog::MessagePtr cmd;
        if (line) {
            if (handler_root) {
                std::string line_s(line);
                line_s.erase(line_s.find_last_not_of(" \t") + 1);
                if (!line_s.empty()) {
                    cmd = llog::parse_cmd(line_s);
                }
            }

            if (strlen(line) > 0) {
                add_history(line); // Add to history
            }
            free(line); // Free the memory allocated by readline
        }
        // Re-install handler for next line
        rl_callback_handler_install(PROMPT, line_handler);

        if (cmd)
            llog::process_chain(handler_root, cmd);

    }

}

llog::ReadlinePtr llog::Readline::create(HandlerChainLinkPtr _handler_root) {

    rl_callback_handler_install(PROMPT, line_handler);
    rl_attempted_completion_function = llog::rl_completion;

    handler_root = std::move(_handler_root);
    ReadlinePtr rl(new Readline);

    rl->m_severity_settings = std::vector<bool>(static_cast<int>(severity::SEVERITY_MAX), true);
    return std::move(rl);
}

bool llog::Readline::handle(MessagePtr msg) {

    static int last_line_repeated = 0;
    static std::string last_line;
    static bool prev_same_line = false;

    if (msg->type() == MessageType::LOG_MSG_GENERIC) {
        auto msg_cast = std::dynamic_pointer_cast<GenericMessage>(msg);
        if (msg_cast) {

            if (!m_severity_settings[static_cast<int>(msg_cast->sev())])
                return false;

            // check filtering
            for (const auto& filter_out : m_filter_outs) {
                if (msg_cast->msg().find(filter_out) != std::string::npos)
                    return false;
            }

            const auto& line = msg_cast->msg();
            bool same_line = last_line == line;
            last_line = line;

            if (same_line)
                last_line_repeated++;

            if (same_line && m_dedup == DedupOnOff::DedupType::ON)
                return false;

            if (same_line && m_dedup == DedupOnOff::DedupType::INFO && last_line_repeated > 4)
                return false;

            char *saved_line = rl_copy_text(0, rl_end);
            int saved_point = rl_point;

            rl_save_prompt();
            rl_replace_line("", 0);
            rl_redisplay();

            if (m_dedup == DedupOnOff::DedupType::OFF) {
                // Print your message
                printf("%s\n", msg_cast->formatted().c_str());
                fflush(stdout);
            } else {
                if (same_line) {
                    // line repeated
                    if (m_dedup == DedupOnOff::DedupType::INFO) {
                        // print how much it has been repeated
                        if (last_line_repeated == 1)
                            printf("\n");

                        if (last_line_repeated <= 3)
                            printf(last_line_repeated == 1 ? "\033[A (repeated %d more time)\n" : "\033[A (repeated %d times)      \n", last_line_repeated);
                        else
                            printf("\033[A (repeating ...)              \n");
                        fflush(stdout);
                    }
                } else {
                    if (prev_same_line != same_line) {
                        printf(last_line_repeated == 1 ? "\033[A (repeated %d more time)\n" : "\033[A (repeated %d times)      \n", last_line_repeated);
                    }
                    last_line_repeated = 0;
                    printf("%s\n", msg_cast->formatted().c_str());
                    fflush(stdout);
                }
                prev_same_line = same_line;
            }

            // Restore input line and prompt
            rl_restore_prompt();
            rl_replace_line(saved_line, 0);
            rl_point = saved_point;
            rl_redisplay();
            free(saved_line);
        }
    }

    if (msg->type() == MessageType::LOG_MSG_TYPE_SERVER_SEVERITY_CHANGE) {
        auto msg_cast = std::dynamic_pointer_cast<SeverityOnOff>(msg);
        if (msg_cast) {
            auto [sev, onoff] = msg_cast->sev_change();
            m_severity_settings[static_cast<int>(sev)] = onoff;
        }
    }

    if (msg->type() == MessageType::LOG_MSG_TYPE_SERVER_DEDUP_CHANGE) {
        auto msg_cast = std::dynamic_pointer_cast<DedupOnOff>(msg);
        if (msg_cast) {
            m_dedup = msg_cast->onoff();
        }
    }

    if (msg->type() == MessageType::LOG_MSG_TYPE_SERVER_FILTEROUT_CHANGE) {
        auto msg_cast = std::dynamic_pointer_cast<CmdParserFilterOut>(msg);
        if (msg_cast) {
            if (!msg_cast->filter_text().empty()) {
                auto _msg =GenericMessage::create(severity::INFO, "system", "now filtering out: " + msg_cast->filter_text());
                process_chain(handler_root, _msg);
                m_filter_outs.push_back(msg_cast->filter_text());
            } else {
                auto _msg =GenericMessage::create(severity::INFO, "system", "filters are cleared");
                process_chain(handler_root, _msg);
                m_filter_outs.clear();
            }
        }
    }
    return false;
}

int llog::Readline::fd() const {
    return STDIN_FILENO;
}

void llog::Readline::read() {
    rl_callback_read_char();
}

llog::Readline::~Readline() {
    rl_callback_handler_remove();
}

