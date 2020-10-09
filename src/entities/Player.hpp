#pragma once

#include <vector>

#include <Player.fwd.hpp>
#include <Map.hpp>
#include <Order.hpp>
#include <Cards.hpp>

using namespace std;

/**
 * A Risk player.
 */
class Player {
private:

    // The territories owned by the players.
    vector<Territory*> territories;

    static int count;
    string name;

    friend std::ostream& operator<<(std::ostream&, const Player&);

public:
    //default constructor
    Player();

    Player(Deck& deck);

    // Copy constructor
    Player(const Player& other);

    // Desctructor.
    ~Player();

    // Orders to apply.
    OrdersList* orders;

    string getName() const;

    // The card collection for the Player.
    Hand* hand;

    void printTerritories() const;

    /**
     * @return A list of territories to defended.
     */
    vector<Territory*> toDefend();

    /**
     * @return A list of territories to attack.
     */
    vector<Territory*> toAttack();


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
     * Add territory to territories.
     */
    void addTerritory(Territory* territory);

    /**
     * Remove territory from territories.
     */
    void removeTerritory(Territory* territory);
};
