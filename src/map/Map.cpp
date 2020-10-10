#include<set>
#include<algorithm>

#include<Map.hpp>

// MapNode
MapNode::MapNode(string name) {
    this->name = name;
}

// Decided to make MapNode shallow clone. Deep clone done in child class.
MapNode::MapNode(MapNode* mapNode) {
    this->name = mapNode->name;
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

vector<Border*> MapNode::getBorders() const {
    return this->borders;
}

bool operator== (const MapNode& m1, const MapNode& m2) {
    return m1.name == m2.name;
}

// Territory


Territory::Territory(string name) : MapNode(name) {
    this->continent = nullptr;
    this->playerOwner = nullptr;
}

Territory::Territory(Territory* territory) : MapNode(territory) {
    Map* m = new Map(territory->getMap());
    Territory* territoryCopy = m->get(territory);

    this->playerOwner = territoryCopy->playerOwner;
    this->numberOfArmies = territoryCopy->numberOfArmies;
    this->continent = territoryCopy->continent;
    this->borders = territoryCopy->borders;
}

Territory::~Territory() {
    if (continent != nullptr) {
        this->continent->remove(this);
    }
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

bool operator==(const Territory& t1, const Territory& t2) {
    return t1.name == t2.name;
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

Map* Territory::getMap() const {
    return this->continent->getMap();
}

// Continent

Continent::Continent(string name) : MapNode(name) {
    this->map = nullptr;
}

Continent::Continent(Continent* continent) : MapNode(continent) {
    Map* m = new Map(continent->getMap());
    Continent* continentCopy = m->get(continent);

    this->territories = continentCopy->territories;
    this->map = continentCopy->map;
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

bool operator== (const Continent& c1, const Continent& c2) {
    return c1.name == c2.name;
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

vector<Territory*> Continent::getTerritories() const {
    return this->territories;
}

Map* Continent::getMap() const {
    return this->map;
}

// Map
Map::Map(string name) {
    this->name = name;
}

Map::Map(Map* m) {
    this->name = m->name;

    // Used for mapping name to object.
    map<string, Continent*> continentMap;
    map<string, Territory*> territoryMap;
    // Used for mapping border name to name.
    vector<pair<string, string>> continentBorders;
    vector<pair<string, string>> territoryBorders;

    for (auto continent : m->continents) {
        string continentName = continent->getName();

        // Making copy of the Continent and attaching to parent Map.
        Continent* continentClone = new Continent(continentName);
        this->connect(continentClone);

        // Mapping name to object.
        continentMap.insert({ continentName, continentClone });

        // Getting all borders (cannot connect automatically because the continent object could not exist).
        for (auto border : continent->getBorders()) {
            continentBorders.push_back({ continentName, border->getOther(continent)->getName() });
        }

        for (auto territory : continent->getTerritories()) {
            string territoryName = territory->getName();

            // Making copy of the Territory and attaching to parent cContinent.
            Territory* territoryClone = new Territory(territoryName);
            continentClone->connect(territoryClone);

            // Mapping name to object.
            territoryMap.insert({ territoryName, territoryClone });

            // Getting all borders (cannot connect automatically because the continent object could not exist).
            for (auto border : territory->getBorders()) {
                territoryBorders.push_back({ territoryName, border->getOther(territory)->getName() });
            }
        }
    }

    // Connecting Continents according to border (we know all objects must exist at this point).
    for (auto border : continentBorders) {
        ((MapNode*)continentMap[border.first])->connect(continentMap[border.second]);
    }

    // Connecting Borders according to border (we know all objects must exist at this point).
    for (auto border : territoryBorders) {
        ((MapNode*)territoryMap[border.first])->connect(territoryMap[border.second]);
    }
}

Map::~Map() {
    while (!this->continents.empty()) delete this->continents.back();
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Map& m) {
    stream << m.name << endl << endl;

    stream << "-----------------" << endl << endl;

    for (auto continent : m.continents) {
        stream << *continent;
    }

    stream << "-----------------" << endl << endl;

    for (auto territory : m.getTerritories()) {
        stream << *territory;
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

vector<Border*> Map::getBorders() const {
    vector<Border*> borders;

    for (auto territory : this->getTerritories()) {
        vector<Border*> temp = territory->getBorders();

        borders.insert(borders.end(), temp.begin(), temp.end());
    }

    return borders;
}

Continent* Map::get(Continent* continent) {
    for(auto c : this->continents) {
        if(*c == *continent) {
            return c;
        }
    }

    return NULL;
}

Territory* Map::get(Territory* territory) {
    for(auto t : this->getTerritories()) {
        if(*t == *territory) {
            return t;
        }
    }

    return NULL;
}

Border* Map::get(Border* border) {
    for(auto b : this->getBorders()) {
        if(*b == *border) {
            return b;
        }
    }

    return NULL;
}