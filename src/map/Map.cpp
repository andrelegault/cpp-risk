#include<Map.hpp>

#include<set>
#include<algorithm>

Map::Map(string name) {
    this->name = name;
}

// TODO: Deep clone?
Map::Map(Map* map) {
    this->name = map->name;
    this->continents = map->continents;
}

Map::~Map() {
    while(!this->continents.empty()) delete this->continents.back();
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Map* m) {
    stream << m->name << endl << endl;

    for (auto continent : m->continents) {
        stream << continent;
    }

    return stream;
}

void Map::operator=(const Map* map) {
    this->~Map();
    this->name = map->name;
    this->continents = map->continents;
}

void Map::connect(Continent* continent) {
    continent->connect(this);
    this->continents.push_back(continent);
}

void Map::remove(Continent* continent) {
    auto p = find(this->continents.begin(), this->continents.end(), continent);

    if(p != this->continents.end()) this->continents.erase(p);
}

bool Map::validate() {
    set<Territory*> seenTerritories;

    for (auto continent : this->continents) {
        if (!continent->validate()) return false;

        for (auto territory : continent->getTerritories()) {
            if (seenTerritories.find(territory) != seenTerritories.end()) return false;

            seenTerritories.insert(territory);
        }
    }

    return true;
}

// TODO: Copy?
vector<Continent*> Map::getContinents() {
    return this->continents;
}

vector<Territory*> Map::getTerritories() {
    vector<Territory*> territories;

    for(auto continent : this->continents) {
        vector<Territory*> temp = continent->getTerritories();

        territories.insert(territories.end(), temp.begin(), temp.end());
    }

    return territories;
}