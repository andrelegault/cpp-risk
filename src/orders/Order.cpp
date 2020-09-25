#include <Order.hpp>

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
        delete order;
        order = nullptr;
    }
}

void OrdersList::remove(const int& index) {
    delete orders[index];
    orders[index] = nullptr;
    orders.erase(orders.begin() + index);
}

void OrdersList::move(const int& prev, const int& next) { }