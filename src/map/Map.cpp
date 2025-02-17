#include "Map.hpp"

/******************************************************
 * MAP
 *****************************************************/

Map::Map() : Map("") {}

Map::Map(const string name) : name(name) {}

Map::Map(const Map& m) {
    this->name = m.name;

    // Used for mapping name to object.
    map<string, Continent*> continentMap;
    map<string, Territory*> territoryMap;

    // Used for mapping border name to name.
    vector<pair<string, string>> continentBorders;
    vector<pair<string, string>> territoryBorders;

    for (auto continent : m.continents) {
        string continentName = continent->getName();
        int continentBonus = continent->getBonus();

        // Making copy of the Continent and attaching to parent Map.
        Continent* continentClone = new Continent(continentName, continentBonus);
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

    delete Map::neutralP;
}

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

Map& Map::operator=(const Map& other) {
    if (&other != this) {
        this->~Map();
        this->name = other.name;
        this->continents = other.continents;
    }

    return *this;
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

Continent* Map::get(const Continent& continent) {
    for (auto c : this->continents) if (*c == continent) return c;

    return nullptr;
}

Territory* Map::get(const Territory& territory) {
    for (auto t : this->getTerritories()) if (*t == territory) return t;

    return nullptr;
}

Border* Map::get(const Border& border) {
    for (auto b : this->getBorders()) if (*b == border) return b;

    return nullptr;
}

Player* Map::neutralP = new Player("Neutral Player", nullptr, new NeutralPlayerStrategy());

/******************************************************
 * MAPNODE
 *****************************************************/

MapNode::MapNode() : MapNode("") {}

MapNode::MapNode(const string name) {
    this->name = name;
}

// Decided to make MapNode shallow clone. Deep clone done in child class.
MapNode::MapNode(const MapNode& mapNode) {
    this->name = mapNode.name;
}

MapNode::~MapNode() {
    while (!this->borders.empty()) {
        Border* border = this->borders.back();
        border->getOther(this)->remove(border);
        delete border;
    }
}

MapNode& MapNode::operator=(const MapNode& other) {
    if (&other != this) {
        borders = other.borders;
        name = other.name;
    }

    return *this;
}

ostream& operator<<(ostream& stream, const MapNode& node) {
    stream << "MapNode with name: " << node.name << endl;

    for (Border* border : node.borders) {
        stream << "\t" << border << endl;
    }

    return stream;
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

/******************************************************
 * TERRITORY
 *****************************************************/

Territory::Territory() : Territory("") {}

Territory::Territory(const string name) : continent(nullptr), playerOwner(nullptr), MapNode(name), numberOfArmies(0) {}

Territory::Territory(const Territory& territory) : MapNode(territory) {
    Map* m = new Map(*(territory.getMap()));
    Territory* territoryCopy = m->get(territory);

    this->playerOwner = territoryCopy->playerOwner;
    this->numberOfArmies = territoryCopy->numberOfArmies;
    this->continent = territoryCopy->continent;
    this->borders = territoryCopy->borders;
}

Territory::~Territory() {
    if (continent != nullptr) this->continent->remove(this);
}

ostream& operator<<(ostream& stream, const Territory& territory) {
    stream << territory.name << " (" << territory.continent->getName() << ")" << endl;

    for (auto border : territory.borders) {
        stream << "-> " << border->getOther((Territory*)&territory)->getName() << endl;
    }

    stream << endl;

    return stream;
}

Territory& Territory::operator=(const Territory& other) {
    if (&other != this) {
        this->~Territory();
        this->name = other.name;
        this->borders = other.borders;
        this->numberOfArmies = other.numberOfArmies;
        this->playerOwner = other.playerOwner;
        this->continent = other.continent;
    }

    return *this;
}

bool operator==(const Territory& t1, const Territory& t2) {
    return t1.name == t2.name;
}

void Territory::connect(Continent* continent) {
    this->continent = continent;
}

void Territory::setPlayerOwner(Player* player) {
    if (this->playerOwner != nullptr) this->playerOwner->removeTerritory(this);

    this->playerOwner = player;
}

string Territory::getOwnerName() {
    return this->playerOwner->getName();
}

bool Territory::validate() {
    return this->borders.size() > 0;
}

Continent* Territory::getContinent() const {
    return this->continent;
}

Map* Territory::getMap() const {
    return this->continent->getMap();
}

Player* Territory::getOwner() const {
    return this->playerOwner;
}

vector<Territory*> Territory::getPlayerBorderTerritories(Player* player) {
    vector<Territory*> territories;

    for (auto border : this->borders) {
        Territory* otherTerritory = (Territory*)border->getOther(this);

        if (otherTerritory->playerOwner == player) {
            territories.push_back(otherTerritory);
        }
    }

    return territories;
}

bool Territory::isNeighbour(Territory* t) {
    for (auto border : this->getBorders())
        if (t == (Territory*)border->getOther(this)) return true;
    return false;
}

bool Territory::operator<(const Territory& other) const {
    return this->numberOfArmies < other.numberOfArmies;
}

bool Territory::comparePointers(const Territory* t1, const Territory* t2) {
    return *t1 < *t2;
}


bool Territory::attack(Territory* target, int attackerArmies, int attackerOdds, int defenderOdds) {
    // TODO: replace with a player whose strategy is NeutralPlayerStrategy
    if (this->getOwner() == Map::neutralP) throw std::runtime_error("neutral player should not be attacking!");
    if (attackerArmies < 0) throw std::runtime_error("Territory attack using negative number of armies.");

    this->setNumberOfArmies(this->getNumberOfArmies() - attackerArmies);

    while (attackerArmies > 0 && target->getNumberOfArmies() > 0) {
        if (rand() % 100 <= attackerOdds) {
            target->setNumberOfArmies(target->getNumberOfArmies() - 1);
        }

        if (target->numberOfArmies > 0) {
            if (rand() % 100 <= defenderOdds) {
                attackerArmies--;
            }
        }
    }

    if (attackerArmies > 0) {
        // attacker won
        target->setNumberOfArmies(attackerArmies);
        target->playerOwner->removeTerritory(target);
        this->playerOwner->addTerritory(target);
        return true;
    }
    else {
        // attacker lost
        return false;
    }
}

std::string Territory::territoryTable(std::vector<Territory*> territories, int width) {
    map<std::string, vector<Territory*>> continentTerritories;

    for (auto territory : territories) {
        continentTerritories[territory->continent->getName()].push_back(territory);
    }

    vector<UI::Component*> data;

    for (auto pair : continentTerritories) {
        data.push_back(new UI::Text(pair.first));

        stringstream ss;

        for (auto territory : pair.second) {
            ss << territory->getName() << " -> " << territory->numberOfArmies << endl;
        }

        data.push_back(new UI::Text(ss.str()));
    }

    vector<vector<UI::Component*>> table;

    vector<UI::Component*> row;

    for (auto component : data) {
        if (row.size() == width) {
            table.push_back(row);
            row = vector<UI::Component*>();
        }

        row.push_back(component);
    }

    table.push_back(row);

    stringstream ss;

    ss << UI::Grid(table);

    return ss.str();
}

void Territory::setNumberOfArmies(int numberOfArmies) {
    if (numberOfArmies < 0) throw runtime_error("Territory " + this->getName() + " army count is being set with invalid value " + std::to_string(numberOfArmies) + ".");

    this->numberOfArmies = numberOfArmies;
}

int Territory::getNumberOfArmies() {
    if (numberOfArmies < 0) throw runtime_error("Territory " + this->getName() + " army count has invalid value " + std::to_string(numberOfArmies) + ".");

    return this->numberOfArmies;
}

/******************************************************
 * CONTIENT
 *****************************************************/

Continent::Continent() : Continent("", 0) {}

Continent::Continent(string name, int bonus) : MapNode(name), bonus(bonus), map(nullptr) { }

Continent::Continent(const Continent& continent) : MapNode(continent), bonus(continent.bonus) {
    Map* m = new Map(*(continent.getMap()));
    Continent* continentCopy = m->get(continent);

    this->territories = continentCopy->territories;
    this->map = continentCopy->map;
}

Continent::~Continent() {
    if (this->map != nullptr) this->map->remove(this);

    while (!this->territories.empty()) delete this->territories.back();
}

ostream& operator<<(ostream& stream, const Continent& continent) {
    stream << continent.name << "(" << continent.bonus << ")" << endl;

    for (auto border : continent.borders) {
        stream << "-> " << border->getOther((Continent*)&continent)->getName() << endl;
    }

    return stream << endl;
}

Continent& Continent::operator=(const Continent& other) {
    if (&other != this) {
        this->~Continent();
        this->name = other.name;
        this->bonus = other.bonus;
        this->borders = other.borders;
        this->map = other.map;
        this->territories = other.territories;
    }
    return *this;
}

bool operator== (const Continent& c1, const Continent& c2) {
    return c1.name == c2.name && c1.bonus == c2.bonus;
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
    for (auto territory : this->territories) if (!territory->validate()) return false;

    return true;
}

vector<Territory*> Continent::getTerritories() const {
    return this->territories;
}

Map* Continent::getMap() const {
    return this->map;
}

const int Continent::getBonus() const {
    return this->bonus;
}

/******************************************************
 * BORDER
 *****************************************************/

Border::Border() : Border(nullptr, nullptr) {}

Border::Border(MapNode* n1, MapNode* n2) {
    this->n1 = n1;
    this->n2 = n2;
}

Border::Border(const Border& border) {
    Map* m = new Map(*(border.getMap()));
    Border* borderCopy = m->get(border);

    this->n1 = borderCopy->n1;
    this->n2 = borderCopy->n2;
}

Border::~Border() {
    if (this->n1 != nullptr) this->n1->remove(this);
    if (this->n2 != nullptr) this->n2->remove(this);
}

ostream& operator<<(ostream& stream, const Border* border) {
    stream << border->n1 << " <-> " << border->n2;

    return stream;
}

bool operator== (const Border& b1, const Border& b2) {
    return *b1.n1 == *b2.n1 && *b1.n2 == *b2.n2;
}

Border& Border::operator=(const Border& other) {
    if (&other != this) {
        this->~Border();
        this->n1 = other.n1;
        this->n2 = other.n2;
    }
    return *this;
}

MapNode* Border::getOther(MapNode* n) {
    if (n == this->n1) return this->n2;
    else return this->n1;
}

bool Border::has(MapNode* node) {
    return node == this->n1 || node == this->n2;
}

Map* Border::getMap() const {
    return this->n1->getMap();
}
