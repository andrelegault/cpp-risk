#pragma once

#include <iostream>
#include <vector>

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
/**
 * Structure to hold orders and apply operations over.
 */
class OrdersList {
private:
    // List of orders.
    vector<Order*> orders;
public:
    /**
     * Default constructor.
     */
    OrdersList() = default;

    /**
     * Parameter constructor.
     */
    OrdersList(vector<Order*>* orders);

    /**
     * Desctuctor.
     */
    ~OrdersList();

    /**
     * Deletes an order from the list
     * @param index Index containing element to be deleted.
     */
    void remove(const int& index);

    /**
     * Moves an order to the provided index.
     * @param index Index to move the order to.
     */
    void move(const int& prev, const int& next);
};