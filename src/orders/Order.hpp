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
     * Default Constructor.
     */
    Order();

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
     * Stream operator to describe order in string format.
     * If order has been executed, it should also output the effect of the order.
     * @param os The ostream to output to.
     * @param order The order to convert to string format.
     * @return Updates ostream.
     */
    friend ostream& operator<<(ostream& os, const Order& order);

    /**
     * String for subclasses.
     */
    virtual string toString() const = 0;

    /**
     * Clone constructor for subclasses.
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

class BlockableOrder : public Order {
public:
    /**
     * Default constructor
     */
    BlockableOrder();
    /**
     * Destructor
     */
    ~BlockableOrder();
    /**
     * Copy constructor
     */
    BlockableOrder(const BlockableOrder& other);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    BlockableOrder& operator=(const BlockableOrder& other);
     /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param target Target territory for the order.
     */
    BlockableOrder(Player* player, Territory* target);
protected:
    /**
     * Check whether the owner of the target territory is immune against the attack.
     */ 
    bool isBlocked();
    /**
     * Target territory for the order.
     */ 
    Territory* target;
};

class Deploy : public Order {
public:
    /**
     * Default constructor
     */
    Deploy();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param target Target territory for the order.
     */
    Deploy(Player* player, Territory* target, int armyCount = 0);
    /**
     * Copy constructor
     */
    Deploy(const Deploy& order);
    /**
     * Describe Deploy order in string format.
     */
    string toString() const;
    /**
     * Stream operator to describe order in string format.
     * If order has been executed, it should also output the effect of the order.
     * @param os The ostream to output to.
     * @param order The order to convert to string format.
     * @return Updates ostream.
     */
    friend ostream& operator<<(ostream& os, const Deploy& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Deploy& operator=(const Deploy& other);
    /**
     * Clone constructor
     */
    Deploy* clone() const;
    /**
     * Destructor
     */
    ~Deploy();
    /**
     * Validate by ensuring that the target territory belongs to the player issuing the order
     */ 
    bool validate() const;
    /**
     * Adds specified number of armies to the territory.
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;
private:
    /**
     * Target territory for the order.
     */ 
    Territory* target;
    /**
     * Number of armies to be deployed
     */
    int armyCount;
};

class Advance : public BlockableOrder {
public:
/**
     * Default constructor
     */
    Advance();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param source Source territory for the order.
     * @param target Target territory for the order.
     */
    Advance(Player* player, Territory* source, Territory* target, int armyCount);
    /**
     * Copy constructor
     */
    Advance(const Advance& order);
    /**
     * Describe Advance order in string format.
     */
    string toString() const;
    /**
     * Stream operator to describe Advance order in string format.
     * @param os The stream to output to.
     * @param order The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const Advance& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Advance& operator=(const Advance& other);
    /**
     * Clone constructor
     */
    Advance* clone() const;
    /**
     * Destructor
     */
    ~Advance();
    /**
     * Validate that the source territory belongs to the issuing player.
     */ 
    bool validate() const;
    /**
     * Move the specified number of army units from source to target territory.
     * Attack is simulated if the target territory belongs to a different player.
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;
    static const int ADVANCE_ORDERS_ISSUED_PER_TURN = 50;
private:
    /**
     * Source territory for the order.
     */ 
    Territory* source;
    /**
     * Number of armies to Advance with
     */
    int armyCount;
};

class Bomb : public BlockableOrder {
public:
/**
     * Default constructor
     */
    Bomb();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param target Target territory for the order.
     */
    Bomb(Player* player, Territory* target);
    /**
     * Copy constructor
     */
    Bomb(const Bomb& order);
    /**
     * Describe Bomb order in string format.
     */
    string toString() const;
     /**
     * Stream operator to describe Bomb order in string format.
     * @param os The stream to output to.
     * @param order The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const Bomb& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Bomb& operator=(const Bomb& other);
    /**
     * Destructor
     */
    ~Bomb();
    /**
     * Clone constructor
     */
    Bomb* clone() const;
    /**
     * Validate that the target territory does not belong to the issuing player.
     */ 
    bool validate() const;
    /**
     * Half the number of armies at the target territory.
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;
};

class Blockade : public Order {
public:
    /**
     * Default constructor
     */
    Blockade();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param target Target territory for the order.
     */
    Blockade(Player* player, Territory* target);
    /**
     * Copy constructor
     */
    Blockade(const Blockade& order);
    /**
     * Describe Blockade order in string format.
     */
    string toString() const;
     /**
     * Stream operator to describe Blockade order in string format.
     * @param os The stream to output to.
     * @param order The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const Blockade& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Blockade& operator=(const Blockade& other);
    /**
     * Destructor
     */
    ~Blockade();
    /**
     * Clone constructor
     */
    Blockade* clone() const;
    /**
     * Validate that the target territory is owned by the order issuing player.
     */ 
    bool validate() const;
    /**
     * Double the number of armies at the target territory and transfer its ownership to the Neutral player
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;
private:
    static const int MULTIPLY_RATIO = 2;
     /**
     * Target territory for the order.
     */ 
    Territory* target;
};

class Airlift : public BlockableOrder {
public:
    /**
     * Default constructor
     */
    Airlift();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param source Source territory for the order.
     * @param target Target territory for the order.
     * @param armyCount Number of army units to be moved.
     */
    Airlift(Player* player, Territory* source, Territory* target, int armyCount);
    /**
     * Copy constructor
     */
    Airlift(const Airlift& order);
    /**
     * Describe Airlift order in string format.
     */
    string toString() const;
     /**
     * Stream operator to describe Airlift order in string format.
     * @param os The stream to output to.
     * @param order The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const Airlift& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Airlift& operator=(const Airlift& other);
    /**
     * Destructor
     */
    ~Airlift();
    /**
     * Clone constructor
     */
    Airlift* clone() const;
    /**
     * Validate the the source and target territories are owned by the issuing player
     */ 
    bool validate() const;
    /**
     * Move specified number of armies from source territory to target territory
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;
private:
    /**
     * Source territory for the order.
     */ 
    Territory* source;
    /**
     * Number of armies to transfer through Airlift
     */
    int armyCount;
};

class Negotiate : public Order {
public:
    /**
     * Default constructor
     */
    Negotiate();
    /**
     * Parameter constructor.
     * @param player Owner of the order.
     * @param target Target territory for the order.
     */
    Negotiate(Player* player, Player* target);
    /**
     * Copy constructor
     */
    Negotiate(const Negotiate& order);
    /**
     * Describe Negotiate order in string format.
     */
    string toString() const;
     /**
     * Stream operator to describe Negotiate order in string format.
     * @param os The stream to output to.
     * @param order The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& os, const Negotiate& order);
    /**
     * Assignment operator overload.
     * @param other Reference used for assignment.
     */
    Negotiate& operator=(const Negotiate& other);
    /**
     * Destructor
     */
    ~Negotiate();
    /**
     * Clone constructor
     */
    Negotiate* clone() const;
    /**
     * Ensure that the target is not the owner of the card.
     */ 
    bool validate() const;
    /**
     * Make the order issuing player and the target player immune to each others' attacks for the rest of the turn
     */ 
    bool execute();
    /**
     * Returns the priority for the order.
     */
    virtual int getPriority() override;

private:
    /**
     * Target player for the order.
     */ 
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
    vector<Order*>::iterator getOrder(Order* order);

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
