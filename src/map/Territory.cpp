#include <Territory.hpp>

#include <algorithm>

Territory::Territory(string name) : MapNode(name) {}

// TODO: Deep clone?
Territory::Territory(Territory* territory) : MapNode(territory) {}

Territory::~Territory() {
    this->continent->remove(this);
}

// TODO: Better print.
ostream& operator<<(ostream& stream, const Territory* territory) {
    stream << territory->name << " (" << territory->continent->getName() << ")" << endl;

    for(auto border : territory->borders) {
        stream << "-> " << border->getOther((Territory*)territory)->getName() << endl;
    }

    return stream << endl;
}

void Territory::connect(Continent* continent) {
    this->continent = continent;
}

/* void Territory::set(Player* player) {
    if(this->playerOwner) this->playerOwner->removeTerritory(this);

    player->addTerritory(this);

    this->playerOwner = player;
} */

bool Territory::validate() {
    return this->borders.size() == 0;
}