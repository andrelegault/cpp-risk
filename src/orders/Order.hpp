#pragma once

#include <iostream>
#include <algorithm>

#include "Order.fwd.hpp"
#include "GameEngine.hpp"
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

    virtual string toString() const = 0;

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

    /**
     * Returns the priority for the order.
     */ 
    virtual int getPriority() = 0;

    friend OrdersList;
protected:
    /// Player issuing the order.
    Player* player;
};

class Deploy : public Order {
public:
    Deploy();
    Deploy(Player* player, Territory* target, int armyCount = 0);
    Deploy(const Deploy& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Deploy& order);
    Deploy& operator=(const Deploy& other);
    Deploy* clone() const;
    ~Deploy();
    bool validate() const;
    bool execute();
    virtual int getPriority() override;
private:
    Territory* target;
    int armyCount;
};

class Advance : public Order {
public:
    Advance();
    Advance(Player* player, Territory* source, Territory* target, int armyCount);
    Advance(const Advance& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Advance& order);
    Advance& operator=(const Advance& other);
    Advance* clone() const;
    ~Advance();
    bool validate() const;
    bool execute();
    virtual int getPriority() override;
private:
    Territory* source;
    Territory* target;
    int armyCount;
};

class Bomb : public Order {
public:
    Bomb();
    Bomb(Player* player, Territory* target);
    Bomb(const Bomb& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Bomb& order);
    Bomb& operator=(const Bomb& other);
    ~Bomb();
    Bomb* clone() const;
    bool validate() const;
    bool execute();
    virtual int getPriority() override;
private:
    Territory* target;
};

class Blockade : public Order {
public:
    Blockade();
    Blockade(Player* player, Territory* target);
    Blockade(const Blockade& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Blockade& order);
    Blockade& operator=(const Blockade& other);
    ~Blockade();
    Blockade* clone() const;
    bool validate() const;
    bool execute();
    virtual int getPriority() override;
private:
    Territory* target;
};

class Airlift : public Order {
public:
    Airlift();
    Airlift(Player* player, Territory* source, Territory* target, int armyCount);
    Airlift(const Airlift& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Airlift& order);
    Airlift& operator=(const Airlift& other);
    ~Airlift();
    Airlift* clone() const;
    bool validate() const;
    bool execute();
    virtual int getPriority() override;
private:
    Territory* source;
    Territory* target;
    int armyCount;
};

class Negotiate : public Order {
public:
    Negotiate();
    Negotiate(Player* player, Player* target);
    Negotiate(const Negotiate& order);
    string toString() const;
    friend ostream& operator<<(ostream& os, const Negotiate& order);
    Negotiate& operator=(const Negotiate& other);
    ~Negotiate();
    Negotiate* clone() const;
    bool validate() const;
    bool execute();
    virtual int getPriority() override;

private:
    Player* target;
};

/**
 * Structure to hold orders and apply operations over.
 */
class OrdersList {
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

    friend Player;

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
     * Gets an Order pointer at a specific index.
     * @param index Index of the desired object.
     * @return Pointer to the Order object, or `nullptr` if not found.
     */
    Order* getAtIndex(const unsigned int index) const;

    /**
     * Deletes an order from the list
     * @param order Order to remove from the list.
     */
    void remove(Order* order);

    /**
     * Adds an order to orders.
     */
    void add(Order* order);

    /**
     * Swaps two orders' position in the list.
     * @param first First order.
     * @param second Second order.
     */
    void move(Order* first, Order* second);

    vector<Order*> getOrders() const;
private:
    /// Set of Order pointers.
    vector<Order*> orders;

    /**
     * Finds an order in orders.
     * @param order Order pointer to find.
     * @return orders.end() if not found
     */
    vector<Order*>::iterator findOrder(Order* order);

    /**
     * Gets the next order, taking into account priority.
     * Priority 1 takes precedence over priority 4.
     *
     * If `wantedPriority` is passed, it will return the next order with that priority.
     *
     * @return the order with the highest priority.
     */
    Order* next(const int wantedPriority = -1) const;
};
