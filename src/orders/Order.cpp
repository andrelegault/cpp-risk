#include "Order.hpp"

using std::endl;
using std::ostream;

// Order

ostream& operator<<(ostream& os, const Order& order) {
    os << order.toString();
    return os;
}

Order::Order(Player* player) : player(player) { }

Order::~Order() {
    cout << "Destroying Order" << endl;
}

Order::Order(const Order& order) : player(order.player) { }

Order& Order::operator=(const Order& order) {
    if (this != NULL) {
        this->player = order.player;
    }
    return *this;
}


// Deploy
Deploy::Deploy() : Order(nullptr) {}

Deploy::Deploy(Player* player, Territory* target, int armyCount) : Order(player), target(target), armyCount(armyCount) {}

Deploy::~Deploy() {}

bool Deploy::validate() const {
    if (this->player == nullptr || this->target == nullptr || this->armyCount < 1) throw runtime_error("Invalid deployment!");
    return this->target->getOwner() == this->player && this->player->armies >= this->armyCount;
}

Deploy::Deploy(const Deploy& order) : Order(order), target(new Territory(*(order.target))) {}

string Deploy::toString() const {
    return "DEPLOY:: " + to_string(this->armyCount) + " units |  -> " + this->target->getName();
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
        this->target->numberOfArmies += this->armyCount;

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
Advance::Advance() : Order(nullptr) {}
Advance::Advance(Player* player, Territory* source, Territory* target, int armyCount) : Order(player), source(source), target(target), armyCount(armyCount) {}

Advance::~Advance() {}

bool Advance::validate() const {
    if (this->player == nullptr || this->source == nullptr || this->target == nullptr) throw runtime_error("Advance order missing parameters.");
    if (this->player != this->source->getOwner()) return false;
    if (this->armyCount >= this->source->numberOfArmies) return false;


    vector<Border*> borders = this->source->getBorders();
    for (auto border : borders) {
        if (border->has(target)) {
            return true;
        }
    }

    return false;
}

Advance::Advance(const Advance& order) : Order(order), source(new Territory(*(order.source))), target(new Territory(*(order.source))) {};

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
    // cout << "Executing an advance order!" << endl;

    if (validate()) {
        bool ownsTarget = this->target->getOwner() == this->player;

        if (ownsTarget) {
            this->source->numberOfArmies -= this->armyCount;
            this->target->numberOfArmies += this->armyCount;
        }
        else {
            const bool successful = this->source->attack(this->target, this->armyCount);
            if (successful) {
                player->hand->draw();
            }
        }

        return true;
    }

    return false;
}

Advance* Advance::clone() const {
    return new Advance(*this);
}

// Bomb
Bomb::Bomb() : Order(nullptr) {}
Bomb::Bomb(Player* player, Territory* target) : Order(player), target(target) {}

Bomb::~Bomb() {}

Bomb::Bomb(const Bomb& order) : Order(order), target(new Territory(*(order.target))) {}

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
    if (this->player == nullptr || this->target == nullptr) throw runtime_error("Something is null in a Bomb order, bad!");
    return this->target->getOwner() != this->player;
}

bool Bomb::execute() {
    // cout << "Executing a bomb order!" << endl;

    if (validate()) {
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
    if (this->player == nullptr || this->target == nullptr) throw runtime_error("Something is null in a Blockade order, bad!");
    return this->target->getOwner() == this->player;
}

bool Blockade::execute() {
    // cout << "Executing a blockade order!" << endl;

    if (validate()) {
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
Airlift::Airlift() : Order(nullptr) {}
Airlift::Airlift(Player* player, Territory* source, Territory* target, int armies) : Order(player), source(source), target(target), armyCount(armies) {}

Airlift::~Airlift() {}

bool Airlift::validate() const {
    if (player == nullptr || source == nullptr || target == nullptr || armyCount == -1) throw runtime_error("Something is null in an Airlift order, bad!");
    return this->source->getOwner() == this->player;
}

Airlift::Airlift(const Airlift& order) : Order(order), source(new Territory(*(order.source))), target(new Territory(*(order.source))), armyCount(armyCount) {};

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
    // cout << "Executing an airlift order!" << endl;

    if (validate()) {
        const bool ownsTarget = this->target->getOwner() == this->player;
        if (ownsTarget) {
            // move armies from source to target
            this->target->numberOfArmies += this->source->numberOfArmies;
            this->source->numberOfArmies = 0;
        }
        else {
            // attac
            const bool successful = this->source->attack(this->target, this->armyCount);
            if (successful) {
                this->player->hand->draw();
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
    if (this->player == nullptr || this->target == nullptr) throw runtime_error("Something is null in a Negotiate order, bad!");
    return this->target != this->player;
}

bool Negotiate::execute() {
    // cout << "Executing a negotiate order!" << endl;

    if (validate()) {
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