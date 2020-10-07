#pragma once

#include <Order.fwd.hpp>
#include <Player.hpp>
#include <iostream>

using namespace std;

class Order {
public:
    /**
     * Stream operator to describe order in string format.
     * If order has been executed, it should also output the effect of the order.
     * @param os The ostream to output to.
     * @param order The order to convert to string format.
     * @return Updates ostream.
     */
    friend ostream& operator<<(ostream& os, const Order& order);

    /**
     * Default constructor.
     */
    Order();

    /**
     * Destructor.
     */
    ~Order();

    /**
     * Copy constructor.
     */
    Order(const Order& order);

    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Order& operator=(const Order& other);

    /**
     * Validates the order.
     * @return True if the order is valid.
     */
    virtual bool validate() = 0;

    /**
     * Validate then executes orders according to player's state and order definition.
     * @param player Player to execute orders on.
     * @return Whether the order was successfully executed.
     */
    virtual bool execute(Player* player) = 0;
};

class Deploy : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

class Advance : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

class Bomb : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

class Blockade : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

class Airlift : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

class Negotiate : public Order {
public:
    bool validate();
    bool execute(Player* player);
};

/**
 * Structure to hold orders and apply operations over.
 */
class OrdersList {
private:
    // Set of Order pointers.
    vector<Order*> orders;

    /**
     * Finds an order in orders.
     * @param order Order pointer to find.
     * @return orders.end() if not found
     */
    vector<Order*>::iterator findOrder(Order* order);
public:
    /**
     * Default constructor.
     */
    OrdersList();

    /**
     * Copy constructor.
     */
    OrdersList(const OrdersList& old);

    /**
     * Desctuctor.
     */
    ~OrdersList();

    /**
     * Adds an order to orders.
     */
    void addOrder(Order* order);

    /**
     * Gets the number of elements in orders.
     * @return Number of elements orders.
     */
    int getLength() const;

    /**
     * Stream operator to describe ordersList in string format.
     * @param os The stream to output to.
     * @param ordersList The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const OrdersList& ordersList);

    /**
     * Assignment operator overload.
     * @param other Other OrdersList used for assignment.
     */
    void operator=(const OrdersList& other);

    /**
     * Deletes an order from the list
     * @param order Order to remove from the list.
     */
    void remove(Order* order);

    /**
     * Gets an Order pointer at a specific index.
     * @param index Index of the desired object.
     * @return Pointer to the Order object, or `nullptr` if not found.
     */
    Order* getAtIndex(const unsigned int index);

    /**
     * Swaps two orders' position in the list.
     * @param first First order.
     * @param second Second order.
     */
    void move(Order* first, Order* second);
};