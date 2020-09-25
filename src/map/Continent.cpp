#include <Continent.hpp>

#include <algorithm>

ostream& operator<<(ostream& stream, const Continent* continent) {
    stream << continent->name;

    return stream;
}

Continent::Continent(string name) : MapNode(name) {}

void Continent::add(Territory* territory) {
    this->territories.push_back(territory);
}

void Continent::remove(Territory* territory) {

}

bool Continent::validate() {
    if (this->borders.size() == 0) return false;

    for (auto territory : this->territories) if (!territory->validate()) return false;

    return true;
}

// TODO: COPY INSTEAD!
vector<Territory*> Continent::getTerritories() {
    return this->territories;
}