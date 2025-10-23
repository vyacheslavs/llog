#include "readline_completion_commands_gen.hpp"
#include "readline_severity_completion.hpp"

#include <memory>
#include <vector>
#include <readline/readline.h>
#include <sstream>

namespace {
    static const char *commands[] = {
        "severity",
        "quit",
        "exit",
        nullptr
    };

    char *command_generator(const char *text, int state) {
        static int list_index, len;
        const char *name;

        if (!state) {
            list_index = 0;
            len = strlen(text);
        }

        while ((name = commands[list_index++])) {
            if (strncmp(name, text, len) == 0)
                return strdup(name);
        }

        return nullptr;
    }

    std::vector<std::string> splitString(const std::string& inputString, char delimiter) {
        std::vector<std::string> words;
        std::stringstream ss(inputString); // Create a stringstream from the input string
        std::string word;

        // Extract words using getline with the specified delimiter
        while (std::getline(ss, word, delimiter)) {
            words.push_back(word);
        }
        return words;
    }
}

char ** llog::rl_completion(const char *text, int start, int end) {

    static std::unique_ptr<llog::completion_handler> handlers[] = {
        std::make_unique<severity_completion>()
    };

    if (start == 0)
        return rl_completion_matches(text, command_generator);

    auto cmd = splitString(rl_line_buffer, ' ');

    for (auto& handler : handlers) {
        if (auto comp_gen = handler->can_complete(cmd))
            return rl_completion_matches(text, comp_gen);
    }

    return nullptr;
}
