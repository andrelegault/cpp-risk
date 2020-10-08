#include <Order.hpp>
#include <algorithm>

using std::endl;
using std::ostream;


// Order

ostream& operator<<(ostream& os, const Order& order) {
    os << &order;
    return os;
}

Order::Order() { }

Order::~Order() {
    cout << "Destroying Order" << endl;
}

Order::Order(const Order& order) { }

Order& Order::operator=(const Order& order) {
    return *this;
}


// Deploy
bool Deploy::validate() { return true; }
bool Deploy::execute(Player* player) {
    if (validate()) {
        cout << "Executing an execute order!" << endl;
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
bool Advance::validate() { return true; }
bool Advance::execute(Player* player) {
    if (validate()) {
        cout << "Executing an advance order!" << endl;
        return true;
    }
    else {
        return false;
    }
}
Advance* Advance::clone() const {
    return new Advance(*this);
}

// Bomb
bool Bomb::validate() { return true; }
bool Bomb::execute(Player* player) {
    if (validate()) {
        cout << "Executing a bomb order!" << endl;
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
bool Blockade::validate() { return true; }
bool Blockade::execute(Player* player) {
    if (validate()) {
        cout << "Executing a blockade order!" << endl;
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
bool Airlift::validate() { return true; }
bool Airlift::execute(Player* player) {
    if (validate()) {
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
bool Negotiate::validate() { return true; }
bool Negotiate::execute(Player* player) {
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

OrdersList::OrdersList(const OrdersList* other) {
    for (auto o : other->orders) {
        // Uses copy constructor since temp isn't defined
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

void OrdersList::operator=(const OrdersList& other) {
    this->orders = other.orders;
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

Order* OrdersList::getAtIndex(const unsigned int index) {
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