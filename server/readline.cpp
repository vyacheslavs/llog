#include "readline.hpp"

#include <cstdlib>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include "server_messages.hpp"

#include "log.hpp"
#include "msglog.hpp"

namespace {

    constexpr auto PROMPT = "llog> ";
    llog::HandlerChainLinkPtr handler_root;

    void line_handler(char *line) {
        if (line) {
            if (handler_root) {
                std::string line_s(line);
                if (line_s == "q" || line_s == "exit") {
                    llog::process_chain(handler_root, llog::ServerShutdownMessage::create());
                }
            }

            if (strlen(line) > 0) {
                add_history(line); // Add to history
            }
            free(line); // Free the memory allocated by readline
        }
        // Re-install handler for next line
        rl_callback_handler_install(PROMPT, line_handler);
    }

}

llog::ReadlinePtr llog::Readline::create(HandlerChainLinkPtr _handler_root) {

    rl_callback_handler_install(PROMPT, line_handler);

    handler_root = std::move(_handler_root);
    ReadlinePtr rl(new Readline);
    return std::move(rl);
}

bool llog::Readline::handle(MessagePtr msg) {
    if (msg->type() == MessageType::LOG_MSG_GENERIC) {
        auto msg_cast = std::dynamic_pointer_cast<GenericMessage>(msg);
        if (msg_cast) {
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

