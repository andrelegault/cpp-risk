#include<Map.hpp>

#include<set>
#include<algorithm>

Map::Map() {}

Map::~Map() {
    for(auto continent : this->continents) delete continent;
}

void Map::add(Continent* continent) {
    this->continents.push_back(continent);
}

bool Map::validate() {
    set<Territory*> seenTerritories;

    for(auto continent : this->continents) {
        if(!continent->validate()) return false;

        for(auto territory : continent->getTerritories()) {
            if(seenTerritories.find(territory) != seenTerritories.end()) return false;

            seenTerritories.insert(territory);
        }
    }

    return true;
}

ostream& operator<<(ostream& stream, const Map* m) {
    for(auto continent : m->continents) {
        stream << continent;
    }

    return stream;
}