#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/select.h>
#include <unistd.h>

void line_handler(char *line) {
    if (line) {
        // printf("You entered: '%s'\n", line);
        if (strlen(line) > 0) {
            add_history(line); // Add to history
        }
        free(line); // Free the memory allocated by readline
    }
    // Re-install handler for next line
    rl_callback_handler_install("prompt> ", line_handler);
}

void print_message(const std::string& msg) {
    // Temporarily save user input line

    char *saved_line = rl_copy_text(0, rl_end);
    int saved_point = rl_point;

    rl_save_prompt();
    rl_replace_line("", 0);
    rl_redisplay();

    // Print your message
    printf("%s\n", msg.c_str());
    fflush(stdout);

    // Restore input line and prompt
    rl_restore_prompt();
    rl_replace_line(saved_line, 0);
    rl_point = saved_point;
    rl_redisplay();
}


int main() {
    rl_callback_handler_install("prompt> ", line_handler);

    fd_set read_fds;
    int max_fd;

    int n = 0;

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        max_fd = STDIN_FILENO; // Or max of all FDs you're watching

        timeval timeout = { 1, 0 };

        if (select(max_fd + 1, &read_fds, NULL, NULL, &timeout) < 0) {
            perror("select");
            break;
        }



        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            rl_callback_read_char(); // Process input from stdin
        } else {
            print_message(std::string("working ...") + std::to_string(n++));
        }

        // Handle other file descriptors here if needed
    }

    rl_callback_handler_remove();
    return 0;
}