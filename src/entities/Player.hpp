#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

#include "Player.fwd.hpp"
#include "Map.hpp"
#include "Order.hpp"
#include "Cards.hpp"
#include "UI.hpp"
#include "MVC.hpp"

using std::cout;
using std::ostream;
using std::endl;

/**
 * A Risk player.
 */
class Player : public Subject {
public:
    //default constructor
    Player();

    Player(Deck* deck);

    // Copy constructor
    Player(const Player& other);

    // Desctructor.
    ~Player();

    // Orders to apply.
    OrdersList* orders;

    string getName() const;

    // The card collection for the Player.
    Hand* hand;

    // Number of territories owned by the player.
    int getNumTerritories() const;

    vector<Territory*> getTerritories() const;

    /**
     * Stream insertion operator.
     * @param stream Stream to output to.
     * @param player Reference to object to output.
     */
    friend ostream& operator<<(ostream& stream, const Player& player);

    /**
     * Assignment operator.
     * @param other Reference to object used for assignment.
     */
    Player& operator=(const Player& other);

    /**
     * @return A list of territories to defended.
     */
    vector<Territory*> toDefend();

    /**
     * @return A list of territories to attack.
     */
    vector<Territory*> toAttack();

    /**
     * @return A list of the Territory that Player owns relative to this territory.
     */
    vector<Territory*> getNeighbourTerritories(Territory* territory);

    /**
     * Creates an Order and adds it to the list of orders.
     */
    void issueOrder();

    /**
     * Adds an order to the player's list of orders.
     * @param order Order to add.
     */
    void addOrder(Order* order);

    /**
     * Removes an order from the player's list of orders.
     * @param order Order to add.
     */
    void removeOrder(Order* order);

    /// Gets the number of orders in the player's order list.
    int remainingOrders() const;

    /**
     * Add territory to territories.
     */
    void addTerritory(Territory* territory);

    /**
     * Remove territory from territories.
     */
    void removeTerritory(Territory* territory);

    /// Gets the iterator of a territory.
    vector<Territory*>::iterator getTerritory(Territory* territory);

    void addArmies(const int newArmies);

    int getArmies();

    /**
     * Gets the player's next order.
     * @return the order with highest priority.
     */
    Order* getNextOrder(const int wantedPriority = -1) const;

private:
    // The territories owned by the players.
    vector<Territory*> territories;

    static int count;
    
    string name;

    int armies;
};