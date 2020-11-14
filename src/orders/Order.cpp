#include <Order.hpp>

using std::endl;
using std::ostream;

// TODO: implement DEEP copy for copy constructor + assignment operator for each class that has a pointer member variable

// Order

ostream& operator<<(ostream& os, const Order& order) {
    os << &order;
    return os;
}

Order::Order(Player* player) : player(player) { }

Order::~Order() {
    cout << "Destroying Order" << endl;
}

Order::Order(const Order& order) { }

Order& Order::operator=(const Order& order) {
    if (this != NULL) {
        this->player = order.player;
    }
    return *this;
}


// TODO: create copy constructor for each derived class
// TODO: override assignment operator for each derived class, see slide 58 in slide set 4

// Deploy
Deploy::Deploy(Player* player, Territory* target) : Order(player), target(target) {}
bool Deploy::validate() const {
    auto ownedTerritories = this->player->getTerritories();
    auto end = ownedTerritories.end();
    return find(ownedTerritories.begin(), end, this->target) != end;
}
bool Deploy::execute() {
    if (validate()) {
        cout << "Executing an execute order!" << endl;
        // TODO: get correct number of armies to add
        this->target->numberOfArmies++;
        return true;
    }
    else {
        return false;
    }
}
Deploy* Deploy::clone() const {
    return new Deploy(*this);
}

// Advance
Advance::Advance(Player* player, Territory* source, Territory* target) : Order(player), source(source), target(target) {}
bool Advance::validate() const {
    auto ownedTerritories = this->player->getTerritories();
    auto end = ownedTerritories.end();
    return find(ownedTerritories.begin(), end, this->source) != end;
}
int Advance::getKilledUnits(int chance, int numAttacker, int numDefender) const {
    int damaged = 0;
    for (int i = 0; i < numAttacker; ++i) {
        const int roll = rand() % 100;
        if (roll <= chance) {
            damaged++;
        }
    }
    return damaged;
}
bool Advance::execute() {
    if (validate()) {
        bool ownsTarget = this->player->hasTerritory(this->target);
        if (ownsTarget) {
            // TODO: move some number of units from source to target
            continue;
        }
        else {
            const int killDefenderUnit = rand() % 100;
            const int sourceArmies = this->source->numberOfArmies;
            const int targetArmies = this->source->numberOfArmies;
            const int killedDefenders = this->getKilledUnits(60, sourceArmies, targetArmies);
            const int killedAttackers = this->getKilledUnits(70, targetArmies, sourceArmies);
            if (killedDefenders >= targetArmies) {
                // conquered territory
                const int attackersLeft = sourceArmies - killedAttackers;
                return true;
            }
        }
        cout << "Executing an advance order!" << endl;
        return false;
    }
    else {
        return false;
    }
}
Advance* Advance::clone() const {
    return new Advance(*this);
}

// Bomb
Bomb::Bomb(Player* player, Territory* target) : Order(player), target(target) {}
bool Bomb::validate() const {
    return !this->player->hasTerritory(this->target);
}
bool Bomb::execute() {
    if (validate()) {
        cout << "Executing a bomb order!" << endl;
        this->target->numberOfArmies /= 2;
        return true;
    }
    else {
        return false;
    }
}
Bomb* Bomb::clone() const {
    return new Bomb(*this);
}

// Blockade
Blockade::Blockade(Player* player, Territory* target) : Order(player), target(target) {}
Blockade::Blockade(const Blockade& other) : Order(other) {}
bool Blockade::validate() const {
    return this->player->hasTerritory(this->target);
}
bool Blockade::execute() {
    if (validate()) {
        cout << "Executing a blockade order!" << endl;
        this->target->numberOfArmies *= 2;
        this->player->removeTerritory(this->target);
        this->
        return true;
    }
    else {
        return false;
    }
}
Blockade* Blockade::clone() const {
    return new Blockade(*this);
}

// Airlift
Airlift::Airlift(Player* player, Territory* source, Territory* target, Deploy* deploy) : Order(player), source(source), target(target), deploy(deploy) {}
bool Airlift::validate() const {
    return this->player->hasTerritory(source) && this->player->hasTerritory(target);
}
bool Airlift::execute() {
    if (validate()) {
        // what? deploy order? advance order? i need to shleep
        cout << "Executing an airlift order!" << endl;
        return true;
    }
    else {
        return false;
    }
}
Airlift* Airlift::clone() const {
    return new Airlift(*this);
}

// Negotiate
Negotiate::Negotiate(Player* player) : Order(player) {}
bool Negotiate::validate() const { return true; }
bool Negotiate::execute() {
    if (validate()) {
        cout << "Executing a negotiate order!" << endl;
        return true;
    }
    else {
        return false;
    }
}
Negotiate* Negotiate::clone() const {
    return new Negotiate(*this);
}


// OrdersList

OrdersList::OrdersList() { }

// TODO: implement copy constructor
OrdersList::OrdersList(const OrdersList& other) {
    for (auto o : other.orders) {
        Order* temp = o->clone();
        orders.push_back(temp);
    }
}

OrdersList::~OrdersList() {
    for (Order* order : orders) {
        delete order;
        order = nullptr;
    }
    orders.clear();
    cout << "Destroying OrdersList" << endl;
}

ostream& operator<<(ostream& os, const OrdersList& ordersList) {
    os << "[ ";
    for (Order* order : ordersList.orders) {
        os << *order << ", ";
    }
    os << "]" << endl;
    return os;
}

OrdersList& OrdersList::operator=(const OrdersList& other) {
    if (&other != this) {
        for (auto o : other.orders) {
            Order* temp = o->clone();
            orders.push_back(temp);
        }
    }
    return *this;
}

void OrdersList::addOrder(Order* what) {
    orders.push_back(what);
}

void OrdersList::remove(Order* order) {
    auto o = findOrder(order);
    orders.erase(o);
    delete order;
    order = nullptr;
}

vector<Order*>::iterator OrdersList::findOrder(Order* order) {
    return find(orders.begin(), orders.end(), order);
}

Order* OrdersList::getAtIndex(const unsigned int index) const {
    return orders.at(index);
}

int OrdersList::getLength() const {
    return orders.size();
}

void OrdersList::move(Order* first, Order* second) {
    auto firstIt = findOrder(first);
    auto secondIt = findOrder(second);
    iter_swap(firstIt, secondIt);
}