#include "readline.hpp"

#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "server_messages.hpp"

#include "log.hpp"
#include "msglog.hpp"
#include "readline_completion_commands_gen.hpp"
#include "cmd_parser.hpp"
#include "cmd_parser_severity.hpp"

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
    if (msg->type() == MessageType::LOG_MSG_GENERIC) {
        auto msg_cast = std::dynamic_pointer_cast<GenericMessage>(msg);
        if (msg_cast) {

            if (!m_severity_settings[static_cast<int>(msg_cast->sev())])
                return false;

            char *saved_line = rl_copy_text(0, rl_end);
            int saved_point = rl_point;

            rl_save_prompt();
            rl_replace_line("", 0);
            rl_redisplay();

            // Print your message
            printf("%s\n", msg_cast->formatted().c_str());
            fflush(stdout);

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

