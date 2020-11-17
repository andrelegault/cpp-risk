#include "Utils.hpp"

std::string Utils::trim(std::string& line) {
    line = (std::regex_replace(line, std::regex("^(\\s|\\r)+|(\\s|\\r)+$"), ""));
    return line;
}

std::string Utils::trim_first_word(const std::string& line, const std::string& delim) {
    return line.substr(line.find_first_of(delim) + 1);
}

std::vector<std::string> Utils::split(const std::string& line, const std::string& delim) {
    std::vector<std::string> result;

    size_t found = line.find(delim);
    size_t startIndex = 0;

    while (found != std::string::npos) {
        std::string temp(line.begin() + startIndex, line.begin() + found);
        result.push_back(temp);
        startIndex = found + delim.size();
        found = line.find(delim, startIndex);
    }

    if (startIndex != line.size()) {
        result.push_back(std::string(line.begin() + startIndex, line.end()));
    }

    return result;
}

std::string Utils::string_center(const std::string s, const int w) {
    if(w < s.length()) return s;

    std::stringstream ss, spaces;
    int pad = w - s.size();
    for (int i = 0; i < pad / 2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();
    if (pad > 0 && pad % 2 != 0)
        ss << " ";
    return ss.str();
}

std::string Utils::string_left(const std::string s, const int w) {
    if(w < s.length()) return s;

    std::stringstream ss;

    ss << s << std::string(w - s.length(), ' ');

    return ss.str();
}

std::string Utils::string_right(const std::string s, const int w) {
    if(w < s.length()) return s;

    std::stringstream ss;

    ss << std::string(w - s.length(), ' ') << s;

    return ss.str();
}

int Utils::getRandom(int min, int max) {
    if (min < 0 || max < 0) {
        throw std::runtime_error("what");
    }
    return min + (rand() % static_cast<int>(max - min + 1));
}
