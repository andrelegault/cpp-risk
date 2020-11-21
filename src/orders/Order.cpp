#include "Order.hpp"

using std::endl;
using std::ostream;

/******************************************************
 * ORDER
 *****************************************************/

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}

Order::Order(Player* player) : player(player) { }

Order::~Order() {}

Order::Order(const Order& order) : player(new Player(*(order.player))) { }

Order& Order::operator=(const Order& order) {
    if (&order != this) {
        if (this->player != nullptr) {
            delete player;
        }
        this->player = new Player(*(order.player));
    }
    return *this;
}

/******************************************************
 * BLOCKABLE ORDER
 *****************************************************/

BlockableOrder::BlockableOrder() : Order(nullptr) { }
BlockableOrder::~BlockableOrder() { }
BlockableOrder::BlockableOrder(const BlockableOrder& other) : Order(new Player(*(other.player))), target(new Territory(*(other.target))) { }
BlockableOrder::BlockableOrder(Player* player, Territory* target) : Order(player), target(target) { }

BlockableOrder& BlockableOrder::operator=(const BlockableOrder& other) {
    Order::operator=(other);
    if (&other != this) {
        if (this->target != nullptr) {
            delete target;
        }
        this->target = new Territory(*(other.target));
    }
    return *this;
}

bool BlockableOrder::isBlocked() {
    tuple<Player*, Player*> current = make_tuple(this->target->getOwner(), this->player);
    const auto it = GameEngine::immunities.find(current);
    const bool exists = it != GameEngine::immunities.end();
    const bool blocked = exists && it->second;
    return blocked;
}

/******************************************************
 * DEPLOY
 *****************************************************/

Deploy::Deploy() : Order(nullptr) {}

Deploy::Deploy(Player* player, Territory* target, int armyCount) : Order(player), target(target), armyCount(armyCount) {}

Deploy::~Deploy() {}

bool Deploy::validate() const {
    if (this->player == nullptr || this->target == nullptr || this->armyCount < 1) return false;
    return this->target->getOwner() == this->player && this->player->armies >= this->armyCount && this->armyCount > 0;
}

Deploy::Deploy(const Deploy& order) : Order(order), target(new Territory(*(order.target))) {}

string Deploy::toString() const {
    return "Deploy " + to_string(this->armyCount) + " unit(s) to " + this->target->getName();
}

ostream& operator<<(ostream& os, const Deploy& order) {
    os << order.toString();
    return os;
}

Deploy& Deploy::operator=(const Deploy& other) {
    Order::operator=(other);
    if (&other != this) {
        if (this->target != nullptr) {
            delete target;
        }
        this->target = new Territory(*(other.target));
    }
    return *this;
}


bool Deploy::execute() {
    if (!this->validate()) return false;

    this->player->armies -= this->armyCount;
    this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->armyCount);

    cout << *this << endl;
    return true;
}

Deploy* Deploy::clone() const {
    return new Deploy(*this);
}

int Deploy::getPriority() {
    return 1;
}

/******************************************************
 * ADVANCE
 *****************************************************/

Advance::Advance() : BlockableOrder(nullptr, nullptr), armyCount(-1) {}
Advance::Advance(Player* player, Territory* source, Territory* target, int armyCount) : BlockableOrder(player, target), source(source), armyCount(armyCount) {}

Advance::~Advance() {}

bool Advance::validate() const {
    if (this->player == nullptr || this->source == nullptr || this->target == nullptr) return false;
    if (this->armyCount < 1) return false;
    if (this->player != this->source->getOwner()) return false;
    if (this->armyCount > this->source->getNumberOfArmies()) return false;

    vector<Border*> borders = this->source->getBorders();
    for (auto border : borders) {
        if (border->has(target)) {
            return true;
        }
    }

    return false;
}

Advance::Advance(const Advance& other) : BlockableOrder(other), source(new Territory(*(other.source))), armyCount(other.armyCount) {};

string Advance::toString() const {
    if (this->source == nullptr || this->target == nullptr) throw std::runtime_error("Invalid Advance Order.");

    return "Advance " + to_string(this->armyCount) + " unit(s) from " + this->source->getName() + " to " + this->target->getName();
}

ostream& operator<<(ostream& os, const Advance& order) {
    os << order.toString();
    return os;
}

Advance& Advance::operator=(const Advance& other) {
    BlockableOrder::operator=(other);
    if (&other != this) {
        if (this->source != nullptr) {
            delete source;
        }
        this->source = new Territory(*(other.source));
        this->armyCount = other.armyCount;
    }

    return *this;
}

bool Advance::execute() {
    if (!validate()) return false;

    bool ownsTarget = this->target->getOwner() == this->player;

    if (ownsTarget) {
        this->source->setNumberOfArmies(this->source->getNumberOfArmies() - this->armyCount);
        this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->armyCount);
    }
    else {
        if (isBlocked()) return true;

        const bool successful = this->source->attack(this->target, this->armyCount);

        if (successful) {
            this->player->hand->draw();
        }
    }

    cout << *this << endl;
    return true;
}

Advance* Advance::clone() const {
    return new Advance(*this);
}

int Advance::getPriority() {
    return 4;
}

/******************************************************
 * BOMB
 *****************************************************/

Bomb::Bomb() : BlockableOrder(nullptr, nullptr) {}
Bomb::Bomb(Player* player, Territory* target) : BlockableOrder(player, target) {}

Bomb::~Bomb() {}

Bomb::Bomb(const Bomb& order) : BlockableOrder(order) {}

string Bomb::toString() const {
    return "Bomb " + this->target->getName();
}

ostream& operator<<(ostream& os, const Bomb& order) {
    os << order.toString();
    return os;
}

Bomb& Bomb::operator=(const Bomb& other) {
    BlockableOrder::operator=(other);
    return *this;
}

bool Bomb::validate() const {
    if (this->player == nullptr || this->target == nullptr) return false;
    return this->target->getOwner() != this->player;
}

bool Bomb::execute() {
    if (!this->validate() || this->isBlocked()) return false;

    if (this->target->getNumberOfArmies() > 1) {
        this->target->setNumberOfArmies(this->target->getNumberOfArmies() / 2);
    }

    cout << *this << endl;
    return true;
}

Bomb* Bomb::clone() const {
    return new Bomb(*this);
}

int Bomb::getPriority() {
    return 3;
}

/******************************************************
 * BLOCKADE
 *****************************************************/

Blockade::Blockade(Player* player, Territory* target) : Order(player), target(target) {}

Blockade::Blockade(const Blockade& order) : Order(order), target(new Territory(*(order.target))) {}

Blockade::~Blockade() {}

string Blockade::toString() const {
    return "Blockade " + this->target->getName();
}

ostream& operator<<(ostream& os, const Blockade& order) {
    os << order.toString();
    return os;
}

Blockade& Blockade::operator=(const Blockade& other) {
    Order::operator=(other);

    if (&other != this) {
        if (this->target != nullptr) delete target;

        this->target = new Territory(*(other.target));
    }

    return *this;
}

bool Blockade::validate() const {
    if (this->player == nullptr || this->target == nullptr) return false;
    return this->target->getOwner() == this->player;
}

bool Blockade::execute() {
    if (!this->validate()) return false;

    if (this->target->getNumberOfArmies() > 0)
        this->target->setNumberOfArmies(this->target->getNumberOfArmies() * 2);

    this->player->removeTerritory(this->target);

    Map::neutralP->addTerritory(this->target);

    cout << *this << endl;
    return true;
}

Blockade* Blockade::clone() const {
    return new Blockade(*this);
}

int Blockade::getPriority() {
    return 2;
}

/******************************************************
 * AIRLIFT
 *****************************************************/

Airlift::Airlift() : BlockableOrder(nullptr, nullptr), source(nullptr), armyCount(-1) {}
Airlift::Airlift(Player* player, Territory* source, Territory* target, int armyCount) : BlockableOrder(player, target), source(source), armyCount(armyCount) {}

Airlift::~Airlift() {}

bool Airlift::validate() const {
    if (player == nullptr || source == nullptr || target == nullptr) return false;
    if (armyCount < 0 || this->source->getNumberOfArmies() <= armyCount) return false;
    return this->source->getOwner() == this->player;
}

Airlift::Airlift(const Airlift& order) : BlockableOrder(order), armyCount(order.armyCount) {};

string Airlift::toString() const {
    return "Airlift " + to_string(this->armyCount) + " unit(s) from " + this->source->getName() + " to " + this->target->getName();
}

ostream& operator<<(ostream& os, const Airlift& order) {
    os << order.toString();
    return os;
}

Airlift& Airlift::operator=(const Airlift& other) {
    Order::operator=(other);
    if (&other != this) {
        if (this->source != nullptr) {
            delete source;
        }
        this->source = new Territory(*(other.source));
        this->armyCount = other.armyCount;
    }

    return *this;
}

bool Airlift::execute() {
    if (!this->validate() || this->isBlocked()) return false;

    const bool ownsTarget = this->target->getOwner() == this->player;

    if (ownsTarget) {
        this->target->setNumberOfArmies(this->target->getNumberOfArmies() + this->source->getNumberOfArmies());
        this->source->setNumberOfArmies(0);
    }
    else {
        const bool successful = this->source->attack(this->target, this->armyCount);

        if (successful) this->player->hand->draw();
    }

    cout << *this << endl;
    return true;
}

Airlift* Airlift::clone() const {
    return new Airlift(*this);
}

int Airlift::getPriority() {
    return 2;
}

/******************************************************
 * NEGOTIATE
 *****************************************************/

Negotiate::Negotiate() : Order(nullptr) {}
Negotiate::Negotiate(Player* player, Player* target) : Order(player), target(target) {}

Negotiate::~Negotiate() {}

Negotiate::Negotiate(const Negotiate& order) : Order(order), target(new Player(*target)) {}

string Negotiate::toString() const {
    return "Negotiate with " + this->target->getName();
}
ostream& operator<<(ostream& os, const Negotiate& order) {
    os << order.toString();
    return os;
}

Negotiate& Negotiate::operator=(const Negotiate& other) {
    Order::operator=(other);
    if (&other != this) {
        if (this->target != nullptr) {
            delete target;
        }
        this->target = new Player(*(other.target));
    }

    return *this;
}

bool Negotiate::validate() const {
    if (this->player == nullptr || this->target == nullptr) return false;
    return this->target != this->player;
}

bool Negotiate::execute() {
    if (!this->validate()) return false;

    GameEngine::immunities[make_tuple(this->player, this->target)] = false;
    GameEngine::immunities[make_tuple(this->target, this->player)] = false;

    cout << *this << endl;
    return true;
}

Negotiate* Negotiate::clone() const {
    return new Negotiate(*this);
}

int Negotiate::getPriority() {
    return 2;
}

/******************************************************
 * ORDERSLIST
 *****************************************************/

OrdersList::OrdersList() { }

OrdersList::OrdersList(const OrdersList& other) {
    for (auto o : other.orders) {
        Order* temp = o->clone();
        orders.push_back(temp);
    }
}

OrdersList::~OrdersList() {
    while (!this->orders.empty()) delete this->orders.back();
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
        if (!this->orders.empty()) {
            for (auto order : orders) delete order;
        }
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
    auto o = getOrder(order);

    if (o != orders.end()) orders.erase(o);
}

vector<Order*>::iterator OrdersList::getOrder(Order* order) {
    return find(orders.begin(), orders.end(), order);
}

Order* OrdersList::getAtIndex(const unsigned int index) const {
    return orders.at(index);
}

int OrdersList::getLength() const {
    return orders.size();
}

void OrdersList::move(Order* first, Order* second) {
    auto firstIt = getOrder(first);
    auto secondIt = getOrder(second);
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
