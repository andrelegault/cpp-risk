#include <MapLoader.hpp>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

string trim(string& line) {
    line = (regex_replace(line, regex("^(\\s|\\r)+|(\\s|\\r)+$"), ""));
    return line;
}

string trim_first_word(const string& str, const string& delim = " \t") {
    return str.substr(str.find_first_of(delim) + 1);
}

vector<string> split(const string& str, const string& delim = " ") {
    vector<string> result;

    size_t found = str.find(delim);
    size_t startIndex = 0;

    while (found != string::npos)
    {
        string temp(str.begin() + startIndex, str.begin() + found);
        result.push_back(temp);
        startIndex = found + delim.size();
        found = str.find(delim, startIndex);
    }
    if (startIndex != str.size())
        result.push_back(string(str.begin() + startIndex, str.end()));
    return result;
}

// TODO - @throw errors instead of exit
// TODO - Check map file has all 3 file components
Map MapLoader::load(const string fileName) {
    ifstream file(fileName);
    if (!file) {
        cerr << "Unable to open file :" + fileName;
        exit(1);
        //return false;
    }

    string line;

    string mapName;
    vector<vector<string> > files;
    vector<Continent*> continents;
    vector<Territory*> countries;

    while (getline(file, line)) {
        // trim leading and training whitespaces
        line = trim(line);

        // When you read useless lines, skip them
        if (line.rfind(';', 0) == 0 || line.empty()) {
            continue;
        }

        // Find the name of the map
        if (line.rfind("name", 0) == 0) {
            mapName = line.substr(line.find_first_of(" \t") + 1);
        }

        // Find related files save to 2d vector for now...
        if (line.find("[files]") == 0) {
            getline(file, line);
            while (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                vector<string> words = split(trim(line));
                files.push_back(words);
                getline(file, line);
            }
        }

        // Create Continents
        if (line.find("[continents]") == 0) {
            getline(file, line);
            while (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                vector<string> words = split(trim(line));
                if (!(words.size() == 3 || words.empty())) {
                    std::cout << "Invalid Continent Format" << std::endl;
                    exit(1);
                }
                // 0 - name
                // 1 - army value
                // 2 - color

                continents.push_back(new Continent(words[0]));
                getline(file, line);
            }
        }

        // Create Territories and connect them to their Continents
        if (line.find("[countries]") == 0) {
            getline(file, line);
            while (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                vector<string> words = split(trim(line));
                if (!(words.size() == 5 || words.empty())) {
                    std::cout << "Invalid Country Format" << std::endl;
                    exit(1);
                }
                // 0 - country #
                // 1 - name
                // 2 - continent #
                // 3 - x
                // 4 - y

                Territory* country = new Territory(words[1]);
                Continent* continent = continents[stoi(words[2]) - 1];
                continent->connect(country);

                countries.push_back(country);
                getline(file, line);
            }
        }

        // Connect MapNodes with eachother to form Borders between Territories
        if (line.find("[borders]") == 0) {
            getline(file, line);
            while (line.find_first_not_of(" \t\n\v\f\r") != std::string::npos) {
                vector<string> words = split(trim(line));
                if (words.size() < 2) {
                    std::cout << "Invalid Border Format" << std::endl;
                    exit(1);
                }
                // 0  - parent country #
                // 1+ - adjacent country #

                MapNode* parent = countries[stoi(words[0]) - 1];

                for (int i = 1; i < words.size(); i++) {
                    parent->connect(countries[stoi(words[i]) - 1]);
                }

                getline(file, line);
            }
        }
    }

    // Create a new map
    Map map(mapName);

    // Connect Continent instances to Map instance.
    for (Continent* continent : continents) {
        map.connect(continent);
    }

    return map;
}