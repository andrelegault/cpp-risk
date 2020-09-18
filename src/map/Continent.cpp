#include <Continent.h>
#include <algorithm>

ostream& operator<<(ostream& stream, const Continent* continent) {
    stream << continent->name;
}

Continent::Continent(string name): MapNode(name) {}

bool Continent::validate() {
    if(this->borders.size() == 0) return false;

    for(auto territory : this->territories) if(!territory->validate()) return false;

    return true;
}

// TODO: COPY INSTEAD!
vector<Territory*> Continent::getTerritories() {
    return this->territories;
}