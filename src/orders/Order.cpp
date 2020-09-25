#include <Order.hpp>

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