#include "MapLoader.hpp"

using namespace std;

MapLoader::MapLoader() = default;
MapLoader::MapLoader(const MapLoader& other) = default;
MapLoader& MapLoader::operator=(const MapLoader& other) = default;

ostream& operator<<(ostream& stream, const MapLoader& maploader) {
    stream << "MapLoader @ " << &maploader << endl;
    return stream;
}

Map* MapLoader::load(const string file_name) {
    ifstream file(file_name);
    if (!file) {
        throw logic_error("Unable to open file at " + file_name);
    }

    string line;
    enum mode {FILES, CONTINENTS, COUNTRIES, BORDERS};
    mode mode;


    string map_name;
    vector<vector<string> > files;
    vector<Continent*> continents;
    vector<Territory*> countries;

    while (!file.eof() && getline(file, line)) {
        // trim leading and training whitespaces
        line = Utils::trim(line);

        // When you read useless lines, skip them
        if (line.rfind(';', 0) == 0 || line.empty()) {
            continue;
        } else if (line.rfind("name", 0) == 0) {
            map_name = line.substr(line.find_first_of(" \t") + 1);
            continue;
        } else if (line.find("[files]") == 0) {
            mode = FILES;
            continue;
        } else if (line.find("[continents]") == 0) {
            mode = CONTINENTS;
            continue;
        } else if (line.find("[countries]") == 0) {
            mode = COUNTRIES;
            continue;
        } else if (line.find("[borders]") == 0) {
            mode = BORDERS;
            continue;
        }


        if (mode == FILES) {  // Find related files save to 2d vector for now...
            vector<string> words = Utils::split(line);
            files.push_back(words);
        } else if (mode == CONTINENTS) { // Create Continents
            vector<string> words = Utils::split(line);
            if (!(words.size() == 3 || words.empty())) {
                throw logic_error("Invalid Continent Format");
            }

            // 0 - name
            // 1 - army value
            // 2 - color

            continents.push_back(new Continent(words[0], stoi(words[1])));
        } else if (mode == COUNTRIES) { // Create Territories and connect them to their Continents
            vector<string> words = Utils::split(line);
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
        } else if (mode == BORDERS) { // Connect MapNodes with eachother to form Borders between Territories
            vector<string> words = Utils::split(line);
            if (words.size() < 2) {
                throw logic_error("Invalid Border Format");
            }

            // 0  - parent country #
            // 1+ - adjacent country #

            MapNode* parent = countries[stoi(words[0]) - 1];

            for (int i = 1; i < words.size(); i++) {
                parent->connect(countries[stoi(words[i]) - 1]);
            }
        }
    }

    // Create a new map
    Map* map = new Map(map_name);

    // Connect Continent instances to Map instance.
    for (Continent* continent : continents) {
        map->connect(continent);
    }

    return map;
}

// Conquest File Reader Adapter
ConquestFileReaderAdapter::MapType ConquestFileReaderAdapter::checkFileType(const string file_name) {
    ifstream file(file_name);
    if (!file) {
        throw logic_error("Unable to open file at " + file_name);
    }

    string line;
    while ( !file.eof() && getline(file, line)) {
        if (line.rfind(';', 0) == 0 || line.empty()) {
            continue;
        } else if (line.find("[Map]") == 0 || line.find("[Continents]") == 0) {
            return ConquestFileReaderAdapter::CONQUEST;
        } else if (line.find("[files]") == 0 || line.find("[continents]") == 0) {
            return ConquestFileReaderAdapter::DOMINATION;
        }
    }
    file.close();
}

Map *ConquestFileReaderAdapter::load(const string file_name) {
    switch (checkFileType(file_name)) {
        case DOMINATION:
            return MapLoader::load(file_name);
        case CONQUEST:
            return ConquestFileReader::load(file_name);
        default:
            throw logic_error("Unsupported Map File: " + file_name + " doesn't appear to be a Domination or Conquest map file.");
    }
}

Map *ConquestFileReader::load(const string file_name) {
    ifstream file(file_name);
    if (!file) {
        throw logic_error("Unable to open file at " + file_name);
    }

    string line;
    enum mode {MAP,CONTINENTS, TERRITORIES};
    mode mode;

    vector<vector<string> > info;
    vector<Continent*> continents;
    vector<Territory*> territories;
    vector<vector<string> > borders;

    while (!file.eof() && getline(file, line)) {
        // trim leading and training whitespaces
        Utils::trim(line);

        // When you read useless lines, skip them
        if (line.empty()) {
            continue;
        } else if (line.find("[Map]") == 0) {
            mode = MAP;
            continue;
        } else if (line.find("[Continents]") == 0) {
            mode = CONTINENTS;
            continue;
        } else if (line.find("[Territories]") == 0) {
            mode = TERRITORIES;
            continue;
        }

        // Check which section are we currently processing
        if (mode == MAP) { // Map Section (key=value)
            vector<string> words = Utils::split(line, "=");
            info.emplace_back(words);
        } else if (mode == CONTINENTS) { // Continents Section (name=score)
            vector<string> words = Utils::split(line, "=");
            // The maximum number of continents that a map can have is 32, we'll ignore this limit
            if (words.size() != 2) {
                throw logic_error("Invalid Continent Format: continent=score");
            }

            continents.emplace_back(new Continent(words[0], stoi(words[1])));
        } else if (mode == TERRITORIES) { // Territories Section (territory with continent and list of bordering territories)
            vector<string> words = Utils::split(line, ",");

            if (words.size() < 5) {
                throw logic_error("Invalid Territory Format: name, x, y, continent, borders...");
            }

            // 0 - name
            // 1 - x
            // 2 - y
            // 3 - continent name
            // 4+ - border territories names

            auto* territory = new Territory(words[0]);

            // Search continents by name to connect it 🙃
            for (Continent* continent: continents){
                if (continent->getName() == Utils::trim(words[3])) {
                    continent->connect(territory);
                    break;
                }
            }

            if (territory->getContinent() == nullptr) {
                throw logic_error("Invalid Territory Format: couldn't find parent Continent ("+words[3]+") for " +words[0]);
            }

            // Save the territory in our territories vector
            territories.emplace_back(territory);
            // Borders need to be collected after we read all the territories, search is done by name
            borders.emplace_back(words.begin() + 4, words.end());
        }
    }

    // Create a new map
    Map* map = new Map(Utils::split(Utils::split(file_name, ".map").front(), "/").back());

    // Connect Continent instances to Map instance
    for (Continent* continent : continents) {
        map->connect(continent);
    }

    // Connect Borders
    for (int i = 0; i < territories.size(); ++i) {
        MapNode* parent = territories[i];
        auto neighbors = borders[i];
        // loop over all the neighbors to connect them
        for (auto border: neighbors) {
            // Loop over all the territories to find the correct one 😡😡😡😡😡😡😡
            // because someone decided using full names in a map format is efficient and not error-prone
            for (auto* territory: territories){
                if (territory->getName() == Utils::trim(border)) {
                    parent->connect(territory);
                    break;
                }
            }
        }
    }

    return map;
}
