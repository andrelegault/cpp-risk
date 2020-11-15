#include "Order.hpp"

using std::endl;
using std::ostream;

// TODO: implement DEEP copy for copy constructor + assignment operator for each class that has a pointer member variable

// Order

ostream& operator<<(ostream& os, const Order& order) {
    os << &order;
    return os;
}

Order::Order(Player* player, const int priority) : player(player), priority(priority) { }

Order::~Order() {
    cout << "Destroying Order" << endl;
}

Order::Order(const Order& order) : player(order.player), priority(order.priority) { }

Order& Order::operator=(const Order& order) {
    if (this != NULL) {
        this->player = order.player;
    }
    return *this;
}


// Deploy
Deploy::Deploy(Player* player, Territory* target) : Order(player, 1), target(target) {}

Deploy::~Deploy() {}

bool Deploy::validate() const {
    return this->target->getOwner() == this->player;
}

Deploy::Deploy(const Deploy& order) : Order(order), target(new Territory(*(order.target))) {}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
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
Advance::Advance(Player* player, Territory* source, Territory* target, int armyCount) : Order(player, 4), source(source), target(target), armyCount(armyCount) {}

Advance::~Advance() {}

bool Advance::validate() const {
    auto borders = this->source->getBorders();
    bool isAdjacent = false;
    for (auto border : borders) {
        if (border->has(target)) {
            isAdjacent = true;
        }
    }
    return isAdjacent && this->source->getOwner() == this->player;
}

Advance::Advance(const Advance& order) : Order(order), source(new Territory(*(order.source))), target(new Territory(*(order.source))) {};

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    this->source = other.source;
    this->target = other.target;

    return *this;
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
        bool ownsTarget = this->target->getOwner() == this->player;
        if (ownsTarget) {
            this->source->numberOfArmies -= this->armyCount;
            this->target->numberOfArmies += this->armyCount;
        }
        else {
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
    }

    return false;
}

Advance* Advance::clone() const {
    return new Advance(*this);
}

// Bomb
Bomb::Bomb(Player* player, Territory* target) : Order(player, 4), target(target) {}

Bomb::~Bomb() {}

Bomb::Bomb(const Bomb& order) : Order(order), target(new Territory(*(order.target))) {}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
}

bool Bomb::validate() const {
    return this->target->getOwner() != this->player;
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
Blockade::Blockade(Player* player, Territory* target) : Order(player, 3), target(target) {}

Blockade::Blockade(const Blockade& order) : Order(order), target(new Territory(*(order.target))) {}

Blockade::~Blockade() {}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
}

bool Blockade::validate() const {
    return this->target->getOwner() == this->player;
}

bool Blockade::execute() {
    if (validate()) {
        cout << "Executing a blockade order!" << endl;
        this->target->numberOfArmies *= 2;
        this->player->removeTerritory(this->target);
        // TODO: transfer ownership to neutral player

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
Airlift::Airlift(Player* player, Territory* source, Territory* target, int armies) : Order(player, 2), source(source), target(target), armyCount(armies) {}

Airlift::~Airlift() {}

bool Airlift::validate() const {
    return this->source->getOwner() == this->player;
}

Airlift::Airlift(const Airlift& order) : Order(order), source(new Territory(*(order.source))), target(new Territory(*(order.source))), armyCount(armyCount) {};

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    this->source = other.source;
    this->target = other.target;
    this->armyCount = other.armyCount;

    return *this;
}

bool Airlift::execute() {
    if (validate()) {
        const bool ownsTarget = this->target->getOwner() == this->player;
        if (ownsTarget) {
            // move armies from source to target
            this->target->numberOfArmies += this->source->numberOfArmies;
            this->source->numberOfArmies = 0;
        }
        else {
            // attac
            // TODO: this->source->attack(this->target);
        }

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
Negotiate::Negotiate(Player* player, Player* target) : Order(player, 4), target(target) {}

Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate& order) : Order(order), target(new Player(*target)) {}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    if (this != NULL) {
        this->target = new Player(*(other.target));
    }

    return *this;
}

bool Negotiate::validate() const {
    return this->target != this->player;
}

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

void OrdersList::add(Order* what) {
    orders.push_back(what);
}

void OrdersList::remove(Order* order) {
    auto o = findOrder(order);
    if (o != orders.end()) {
        orders.erase(o);
        delete order;
        order = nullptr;
    }
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

Order* OrdersList::next(const int wantedPriority) const {
    const int numOrders = orders.size();
    if (numOrders > 0) {
        if (wantedPriority == -1) {
            // by regular priority
            Order* highest = orders.front();
            for (int i = 1; i < numOrders; ++i) {
                Order* cur = orders.at(i);
                if (highest->priority > cur->priority) {
                    highest = cur;
                }
            }
            return highest;
        }
        else {
            // with specific priority
            for (auto order : orders) {
                if (order->priority == wantedPriority) {
                    return order;
                }
            }
            return NULL;
        }
    }
    else {
        return NULL;
    }
}