#include <Continent.hpp>

#include <algorithm>

Continent::Continent(string name) : MapNode(name) {}

// TODO: Deep clone?
Continent::Continent(Continent* continent) : MapNode(continent) {
    this->territories = continent->territories;
    this->map = continent->map;
}

Continent::~Continent() {
    this->map->remove(this);

    while(!this->territories.empty()) delete this->territories.back();
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Continent* continent) {
    stream << continent->name << endl;

    for(auto border : continent->borders) {
        stream << "-> " << border->getOther((Continent*)continent)->getName() << endl;
    }

    return stream << endl;
}

void Continent::operator=(const Continent* continent) {
    this->~Continent();
    this->name = continent->name;
    this->borders = continent->borders;
    this->map = continent->map;
    this->territories = continent->territories;
}

void Continent::connect(Map* map) {
    this->map = map;
}

void Continent::connect(Territory* territory) {
    territory->connect(this);

    this->territories.push_back(territory);
}

void Continent::remove(Territory* territory) {
    auto p = find(this->territories.begin(), this->territories.end(), territory);

    if(p != this->territories.end()) this->territories.erase(p);
}

bool Continent::validate() {
    if (this->borders.size() == 0) return false;

    for (auto territory : this->territories) if (!territory->validate()) return false;

    return true;
}

// TODO: Copy?
vector<Territory*> Continent::getTerritories() {
    return this->territories;
}