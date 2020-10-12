#include <MapLoader.hpp>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

MapLoader::MapLoader() { }

MapLoader::MapLoader(const MapLoader& other) { }

MapLoader& MapLoader::operator=(const MapLoader& other) { return *this; }

ostream& operator<<(ostream& stream, const MapLoader& maploader) {
    stream << "MapLoader @ " << &maploader << endl;
    return stream;
}

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

    while (found != string::npos)
    {
        string temp(line.begin() + startIndex, line.begin() + found);
        result.push_back(temp);
        startIndex = found + delim.size();
        found = line.find(delim, startIndex);
    }
    if (startIndex != line.size())
        result.push_back(string(line.begin() + startIndex, line.end()));
    return result;
}

Map MapLoader::load(const string file_name) {
    ifstream file(file_name);
    if (!file) {
        throw logic_error("Unable to open file at " + file_name);
    }

    string line;

    string map_name;
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
            map_name = line.substr(line.find_first_of(" \t") + 1);
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
                    throw logic_error("Invalid Continent Format");
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
                    throw logic_error("Invalid Country Format");
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
                    throw logic_error("Invalid Border Format");
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
    Map map(map_name);

    // Connect Continent instances to Map instance.
    for (Continent* continent : continents) {
        map.connect(continent);
    }

    return map;
}