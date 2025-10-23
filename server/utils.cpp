#include "utils.h"

#include <iostream>
#include <sstream>

std::vector<std::string> llog::utils::splitString(const std::string &inputString, char delimiter) {
    std::vector<std::string> words;
    std::stringstream ss(inputString); // Create a stringstream from the input string
    std::string word;

    // Extract words using getline with the specified delimiter
    while (std::getline(ss, word, delimiter)) {
        words.push_back(word);
    }
    return words;
}

std::string llog::utils::create_session_path(const std::string &session_hint) {
    auto hint = session_hint;
    if (hint.empty()) {
        hint = "session_XXXXXX";
        mktemp(hint.data());
    }

    hint = "/tmp/" + hint;

    return hint;
}
