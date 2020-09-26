#include <iostream>
#include <Order.hpp>

using std::endl;
using std::ostream;

ostream& operator<<(ostream& os, const Order& order) {
    os << &order << endl;
    return os;
}

Order::~Order() {}

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

OrdersList::OrdersList(vector<Order*>* orders) : orders(*orders) {}

OrdersList::~OrdersList() {
    for (Order* order : orders) {
        order = nullptr;
        delete order;
    }
}

ostream& operator<<(ostream& os, const OrdersList& ordersList) {
    for (Order* order : ordersList.orders) {
        os << *order << endl;
    }
    return os;
}

void OrdersList::remove(const unsigned int index) {
    orders[index] = nullptr;
    orders.erase(orders.begin() + index);
}

void OrdersList::move(const unsigned int prev, const unsigned int next) { }