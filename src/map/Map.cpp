#include<set>
#include<algorithm>

#include<Map.hpp>

// MapNode
MapNode::MapNode(string name) {
    this->name = name;
}

// TODO: Deep clone.
MapNode::MapNode(MapNode* mapNode) {
    this->name = mapNode->name;
    this->borders = mapNode->borders;
}

MapNode::~MapNode() {
    while (!this->borders.empty()) delete this->borders.back();
}

string MapNode::getName() {
    return this->name;
}

void MapNode::connect(Border* border) {
    this->borders.push_back(border);
}

void MapNode::remove(Border* border) {
    auto it = find(this->borders.begin(), this->borders.end(), border);

    if (it != this->borders.end()) this->borders.erase(it);
}

void MapNode::connect(MapNode* node) {
    Border* border = new Border(this, node);

    for (auto b : this->borders) {
        if (b->has(node)) return;
    }

    this->connect(border);
    node->connect(border);
}

// Territory


Territory::Territory(string name) : MapNode(name) {}

// TODO: Deep clone?
Territory::Territory(Territory* territory) : MapNode(territory) {
    this->playerOwner = territory->playerOwner;
    this->numberOfArmies = territory->numberOfArmies;
    this->continent = territory->continent;
}

Territory::~Territory() {
    this->continent->remove(this);
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Territory& territory) {
    stream << territory.name << " (" << territory.continent->getName() << ")" << endl;

    for (auto border : territory.borders) {
        stream << "-> " << border->getOther((Territory*)&territory)->getName() << endl;
    }

    return stream << endl;
}

void Territory::operator=(const Territory* territory) {
    this->~Territory();
    this->name = territory->name;
    this->borders = territory->borders;
    this->numberOfArmies = territory->numberOfArmies;
    this->playerOwner = territory->playerOwner;
    this->continent = territory->continent;
}

void Territory::connect(Continent* continent) {
    this->continent = continent;
}

void Territory::set(Player* player) {
    if (this->playerOwner) this->playerOwner->removeTerritory(this);

    player->addTerritory(this);

    this->playerOwner = player;
}

bool Territory::validate() {
    return this->borders.size() > 0;
}

// Continent

Continent::Continent(string name) : MapNode(name) {}

// TODO: Deep clone?
Continent::Continent(Continent* continent) : MapNode(continent) {
    this->territories = continent->territories;
    this->map = continent->map;
}

Continent::~Continent() {
    if (this->map != NULL) {
        this->map->remove(this);
    }

    while (!this->territories.empty()) delete this->territories.back();
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Continent& continent) {
    stream << continent.name << endl;

    for (auto border : continent.borders) {
        stream << "-> " << border->getOther((Continent*)&continent)->getName() << endl;
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

    if (p != this->territories.end()) this->territories.erase(p);
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

// Map
Map::Map(string name) {
    this->name = name;
}

// TODO: Deep clone?
Map::Map(Map* map) {
    this->name = map->name;
    this->continents = map->continents;
}

Map::~Map() {
    while (!this->continents.empty()) delete this->continents.back();
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Map& m) {
    stream << m.name << endl << endl;

    for (auto continent : m.continents) {
        stream << *continent;
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

    if (p != this->continents.end()) this->continents.erase(p);
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
vector<Continent*> Map::getContinents() const {
    return this->continents;
}

vector<Territory*> Map::getTerritories() const {
    vector<Territory*> territories;

    for (auto continent : this->continents) {
        vector<Territory*> temp = continent->getTerritories();

        territories.insert(territories.end(), temp.begin(), temp.end());
    }

    return territories;
}