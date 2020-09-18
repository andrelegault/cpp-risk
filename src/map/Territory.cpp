#include <Territory.hpp>

Territory::Territory(string name): MapNode(name) {}

ostream& operator<<(ostream& stream, const Territory* territory) {
    stream << territory->name;

    return stream;
}

/**
void Territory::set(Player* player) { 
    if(this->playerOwner) this->playerOwner->removeTerritory(this);
    
    player->addTerritory(this);

    this->playerOwner = player;
}*/

bool Territory::validate() {
    return this->borders.size() == 0;
}