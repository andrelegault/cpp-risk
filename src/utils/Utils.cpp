#include <Utils.hpp>

string trim(string& line) {
    line = (regex_replace(line, regex("^(\\s|\\r)+|(\\s|\\r)+$"), ""));
    return line;
}

string trim_first_word(const string& line, const string& delim) {
    return line.substr(line.find_first_of(delim) + 1);
}

vector<string> split(const string& line, const string& delim) {
    vector<string> result;

    size_t found = line.find(delim);
    size_t startIndex = 0;

    while (found != string::npos) {
        string temp(line.begin() + startIndex, line.begin() + found);
        result.push_back(temp);
        startIndex = found + delim.size();
        found = line.find(delim, startIndex);
    }

    if (startIndex != line.size()) {
        result.push_back(string(line.begin() + startIndex, line.end()));
    }

    return result;
}

string string_center(const string s, const int w) {
    stringstream ss, spaces;
    int pad = w - s.size();
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str();
    if(pad>0 && pad%2!=0)
        ss << " ";
    return ss.str();
}

string string_left(const string s, const int w) {
    stringstream ss;

    ss << s << string(w - s.length(), ' ');

    return ss.str();
}

string string_right(const string s, const int w) {
    stringstream ss;

    ss << string(w - s.length(), ' ') << s;

    return ss.str();
}