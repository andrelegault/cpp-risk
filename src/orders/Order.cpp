#include <iostream>
#include <algorithm>
#include <Order.hpp>

using std::endl;
using std::ostream;

ostream& operator<<(ostream& os, const Order& order) {
    os << &order << endl;
    return os;
}

Order::Order() { }

Order::~Order() {}

Order::Order(const Order& order) { }

Order& Order::operator=(const Order& order) {
    return *this;
}

bool Deploy::validate() { return true; }
bool Deploy::execute(Player* player) { return true; }

bool Advance::validate() { return true; }
bool Advance::execute(Player* player) { return true; }

bool Bomb::validate() { return true; }
bool Bomb::execute(Player* player) { return true; }

bool Blockade::validate() { return true; }
bool Blockade::execute(Player* player) { return true; }

bool Airlift::validate() { return true; }
bool Airlift::execute(Player* player) { return true; }

bool Negotiate::validate() { return true; }
bool Negotiate::execute(Player* player) { return true; }

OrdersList::OrdersList() : orders(new vector<Order*>()) { }

OrdersList::OrdersList(vector<Order*>* orders) : orders(orders) {}

OrdersList::OrdersList(const OrdersList& old) : orders(old.orders) {
    // TODO: implement copy constructor
}

OrdersList::~OrdersList() {
    orders = nullptr;
    delete orders;
}

ostream& operator<<(ostream& os, const OrdersList& ordersList) {
    for (Order* order : *(ordersList.orders)) {
        os << *order << endl;
    }
    return os;
}

void OrdersList::operator=(const OrdersList& other) {
    this->orders = other.orders;
}
}

void OrdersList::remove(Order* order) {
    auto o = find(orders->begin(), orders->end(), order);
    orders->erase(o);
}

void OrdersList::move(const unsigned int prev, const unsigned int next) { }

int OrdersList::getLength() {
    return orders->size();
}