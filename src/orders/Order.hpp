#pragma once

#include <iostream>
#include <algorithm>

#include "Order.fwd.hpp"
#include "Player.hpp"

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
     * Parameter constructor.
     * @param player Owner of the order.
     */
    Order(Player* player);

    /**
     * Destructor.
     */
    virtual ~Order() = 0;

    /**
     * Copy constructor.
     */
    Order(const Order& order);

    /**
     * Clone constructor.
     */
    virtual Order* clone() const = 0;

    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Order& operator=(const Order& other);

    /**
     * Validates the order.
     * @return True if the order is valid.
     */
    virtual bool validate() const = 0;

    /**
     * Validate then executes orders according to player's state and order definition.
     * @param player Player to execute orders on.
     * @return Whether the order was successfully executed.
     */
    virtual bool execute() = 0;
    Player* player;
};

class Deploy : public Order {
private:
    Territory* target;
public:
    Deploy(Player* player, Territory* target);
    Deploy(const Deploy& order);
    Deploy& operator=(const Deploy& other);
    Deploy* clone() const;
    ~Deploy();
    bool validate() const;
    bool execute();
};

class Advance : public Order {
public:
    Advance(Player* player, Territory* source, Territory* target);
    Advance(const Advance& order);
    Advance& operator=(const Advance& other);
    Advance* clone() const;
    ~Advance();
    bool validate() const;
    bool execute();
    int getKilledUnits(int chance, int numAttacker, int numDefender) const;
private:
    Territory* source;
    Territory* target;
};

class Bomb : public Order {
private:
    Territory* target;
public:
    Bomb(Player* player, Territory* target);
    Bomb(const Bomb& order);
    Bomb& operator=(const Bomb& other);
    ~Bomb();
    Bomb* clone() const;
    bool validate() const;
    bool execute();
};

class Blockade : public Order {
private:
    Territory* target;
public:
    Blockade(Player* player, Territory* target);
    Blockade(const Blockade& order);
    Blockade& operator=(const Blockade& other);
    ~Blockade();
    Blockade* clone() const;
    bool validate() const;
    bool execute();
};

class Airlift : public Order {
private:
    Territory* source;
    Territory* target;
    Deploy* deploy;
public:
    Airlift(Player* player, Territory* source, Territory* target, Deploy* deploy);
    Airlift(const Airlift& order);
    Airlift& operator=(const Airlift& other);
    ~Airlift();
    Airlift* clone() const;
    bool validate() const;
    bool execute();
};

class Negotiate : public Order {
public:
    Negotiate(Player* player);
    Negotiate(const Negotiate& order);
    Negotiate& operator=(const Negotiate& other);
    ~Negotiate();
    Negotiate* clone() const;
    bool validate() const;
    bool execute();
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
    OrdersList(const OrdersList& other);

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
    friend ostream& operator<<(ostream& os, const OrdersList& other);

    /**
     * Assignment operator overload.
     * @param other Other OrdersList used for assignment.
     * @return Copied reference to OrdersList.
     */
    OrdersList& operator=(const OrdersList& other);

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
    Order* getAtIndex(const unsigned int index) const;

    /**
     * Swaps two orders' position in the list.
     * @param first First order.
     * @param second Second order.
     */
    void move(Order* first, Order* second);
};
