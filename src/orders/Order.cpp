#include "Order.hpp"

using std::endl;
using std::ostream;

// Order

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}

Order::Order(Player* player) : player(player) { }

Order::~Order() {}

Order::Order(const Order& order) : player(order.player) { }

Order& Order::operator=(const Order& order) {
    if (this != NULL) {
        this->player = order.player;
    }
    return *this;
}

BlockableOrder::BlockableOrder() : Order(nullptr) { }
BlockableOrder::BlockableOrder(const BlockableOrder& other) : Order(other.player), target(other.target), armyCount(other.armyCount) { }
BlockableOrder::BlockableOrder(Player* player, Territory* target) : Order(player), target(target) { }

bool BlockableOrder::isBlocked() {
    tuple<Player*, Player*> current = make_tuple(this->target->getOwner(), this->player);
    const auto it = GameEngine::immunities.find(current);
    const bool exists = it != GameEngine::immunities.end();
    const bool blocked = exists && it->second;
    return blocked;
}


// Deploy
Deploy::Deploy() : Order(nullptr) {}

Deploy::Deploy(Player* player, Territory* target, int armyCount) : Order(player), target(target), armyCount(armyCount) {}

Deploy::~Deploy() {}

bool Deploy::validate() const {
    if (this->player == nullptr || this->target == nullptr || this->armyCount < 1) return false;
    return this->target->getOwner() == this->player && this->player->armies >= this->armyCount && this->armyCount > 0;
}

Deploy::Deploy(const Deploy& order) : Order(order), target(new Territory(*(order.target))) {}

string Deploy::toString() const {
    return "DEPLOY:: " + to_string(this->armyCount) + " units | " + this->target->getName();
}

ostream& operator<<(ostream& os, const Deploy& order) {
    os << order.toString() << endl;
    return os;
}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
}


bool Deploy::execute() {
    // cout << "Executing a deploy order!" << endl;
    if (validate()) {
        this->player->armies -= this->armyCount;
        this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->armyCount);

        return true;
    }
    else {
        return false;
    }
}

Deploy* Deploy::clone() const {
    return new Deploy(*this);
}

int Deploy::getPriority() {
    return 1;
}

// Advance
Advance::Advance() : BlockableOrder(nullptr, nullptr), armyCount(-1) {}
Advance::Advance(Player* player, Territory* source, Territory* target, int armyCount) : BlockableOrder(player, target), source(source), armyCount(armyCount) {}

Advance::~Advance() {}

bool Advance::validate() const {
    if (this->player == nullptr || this->source == nullptr || this->target == nullptr) return false;
    if (this->player != this->source->getOwner()) return false;
    if (this->armyCount >= this->source->getNumberOfArmies()) return false;
    if (this->armyCount < 0) return false;

    vector<Border*> borders = this->source->getBorders();
    for (auto border : borders) {
        if (border->has(target)) {
            return true;
        }
    }

    return false;
}

Advance::Advance(const Advance& other) : BlockableOrder(other) {};

string Advance::toString() const {
    if (this->source != nullptr && this->target != nullptr) {
        return "ADVANCE:: " + to_string(this->armyCount) + " units | " + this->source->getName() + " -> " + this->target->getName();
    }
    else {
        throw "ADVANCE:: Invalid Advance";
    }
}

ostream& operator<<(ostream& os, const Advance& order) {
    os << order.toString();
    return os;
}

Advance& Advance::operator=(const Advance& other) {
    Order::operator=(other);
    this->source = other.source;
    this->target = other.target;

    return *this;
}


bool Advance::execute() {
    if (validate()) {
        bool ownsTarget = this->target->getOwner() == this->player;

        if (ownsTarget) {
            this->source->setNumberOfArmies(this->source->getNumberOfArmies() - this->armyCount);
            this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->armyCount);
        }
        else {
            if (!isBlocked()) {
                // skip since immune
                const bool successful = this->source->attack(this->target, this->armyCount);
                if (successful) {
                    this->player->hand->draw();
                }
            }
            else {
                cout << "blocked an advance order from " << this->player->getName() << " to " << this->target->getOwner()->getName() << endl;
            }
        }

        return true;
    }

    return false;
}

Advance* Advance::clone() const {
    return new Advance(*this);
}

int Advance::getPriority() {
    return 4;
}

// Bomb
Bomb::Bomb() : BlockableOrder(nullptr, nullptr) {}
Bomb::Bomb(Player* player, Territory* target) : BlockableOrder(player, target) {}

Bomb::~Bomb() {}

Bomb::Bomb(const Bomb& order) : BlockableOrder(order.player, order.target) {}

string Bomb::toString() const {
    return "BOMB:: " + this->target->getName();
}

ostream& operator<<(ostream& os, const Bomb& order) {
    os << order.toString();
    return os;
}

Bomb& Bomb::operator=(const Bomb& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
}

bool Bomb::validate() const {
    if (this->player == nullptr || this->target == nullptr) return false;
    return this->target->getOwner() != this->player;
}

bool Bomb::execute() {
    if (validate()) {
        // cant bomb if num is 1 or 0
        if (!isBlocked()) {
            if (this->target->getNumberOfArmies() > 1) {
                this->target->setNumberOfArmies(this->target->getNumberOfArmies() / 2);
            }

            return true;
        }
        else {
            cout << "blocked a bomb order from " << this->player->getName() << " to " << this->target->getOwner()->getName() << endl;
            return false;
        }
    }
    else {
        return false;
    }
}

Bomb* Bomb::clone() const {
    return new Bomb(*this);
}

int Bomb::getPriority() {
    return 3;
}

// Blockade
Blockade::Blockade(Player* player, Territory* target) : Order(player), target(target) {}

Blockade::Blockade(const Blockade& order) : Order(order), target(new Territory(*(order.target))) {}

Blockade::~Blockade() {}

string Blockade::toString() const {
    return "BLOCKADE:: " + this->target->getName();
}

ostream& operator<<(ostream& os, const Blockade& order) {
    os << order.toString();
    return os;
}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);
    this->target = other.target;

    return *this;
}

bool Blockade::validate() const {
    if (this->player == nullptr || this->target == nullptr) return false;
    return this->target->getOwner() == this->player;
}

bool Blockade::execute() {
    if (validate()) {
        if (this->target->getNumberOfArmies() > 0)
            this->target->setNumberOfArmies(this->target->getNumberOfArmies() * 2);
        this->player->removeTerritory(this->target);
        // TODO: transfer ownership to neutral player
        //transfer ownership to the neutral player
        this->target->getMap()->neutralP->addTerritory(this->target);
        //neutral territories have 2 armies each
        this->target->numberOfArmies = 2;

        return true;
    }
    else {
        return false;
    }
}

Blockade* Blockade::clone() const {
    return new Blockade(*this);
}

int Blockade::getPriority() {
    return 2;
}

// Airlift
Airlift::Airlift() : BlockableOrder(nullptr, nullptr), source(nullptr), armyCount(-1) {}
Airlift::Airlift(Player* player, Territory* source, Territory* target, int armies) : BlockableOrder(player, target), source(source), armyCount(armyCount) {}

Airlift::~Airlift() {}

bool Airlift::validate() const {
    if (player == nullptr || source == nullptr || target == nullptr || armyCount < 0) return false;
    return this->source->getOwner() == this->player;
}

Airlift::Airlift(const Airlift& order) : BlockableOrder(order), armyCount(armyCount) {};

string Airlift::toString() const {
    return "AIRLIFT:: " + to_string(this->armyCount) + " units | " + this->source->getName() + " -> " + this->target->getName();
}

ostream& operator<<(ostream& os, const Airlift& order) {
    os << order.toString();
    return os;
}

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
            this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->source->getNumberOfArmies());
            this->source->setNumberOfArmies(0);
        }
        else {
            if (!isBlocked()) {
                const bool successful = this->source->attack(this->target, this->armyCount);
                if (successful) {
                    this->player->hand->draw();
                }
            }
            else {
                cout << "blocked an airlift order from " << this->player->getName() << " to " << this->target->getOwner()->getName() << endl;
            }
        }
        return true;
    }
    else {
        return false;
    }
}

Airlift* Airlift::clone() const {
    return new Airlift(*this);
}

int Airlift::getPriority() {
    return 2;
}

// Negotiate
Negotiate::Negotiate() : Order(nullptr) {}
Negotiate::Negotiate(Player* player, Player* target) : Order(player), target(target) {}

Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate& order) : Order(order), target(new Player(*target)) {}

string Negotiate::toString() const {
    return "NEGOTIATE:: " + this->target->getName();
}
ostream& operator<<(ostream& os, const Negotiate& order) {
    os << order.toString();
    return os;
}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    if (this != NULL) {
        this->target = new Player(*(other.target));
    }

    return *this;
}

bool Negotiate::validate() const {
    return (this->player != nullptr && this->target != nullptr) && this->target != this->player;
}

bool Negotiate::execute() {
    if (validate()) {
        GameEngine::immunities[make_tuple(this->player, this->target)] = false;
        return true;
    }
    else {
        return false;
    }
}

Negotiate* Negotiate::clone() const {
    return new Negotiate(*this);
}

int Negotiate::getPriority() {
    return 2;
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

    if (o != orders.end()) orders.erase(o);
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
    if (this->orders.size() > 0) {
        if (wantedPriority == -1) {
            // by regular priority
            Order* highest = nullptr;

            for (auto order : this->orders) {
                if (order != nullptr) {
                    if (highest == nullptr) {
                        highest = order;
                    }
                    else if (highest->getPriority() > order->getPriority()) {
                        highest = order;
                    }
                }
            }

            return highest;
        }
        else {
            // with specific priority
            for (auto order : this->orders) {
                if (order != nullptr) {
                    if (order->getPriority() == wantedPriority) {
                        return order;
                    }
                }
            }
        }
    }

    return nullptr;
}

vector<Order*> OrdersList::getOrders() const {
    return this->orders;
}