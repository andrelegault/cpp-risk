#pragma once

#include <string>
#include <vector>
#include <MapNode.hpp>
#include <Border.hpp>
#include <Player.hpp>
#include <Continent.hpp>

using namespace std;

/**
 * Node for the Map graph.
 */
class Territory : public MapNode {
private:
    // Current player that owns this country.
    Player* playerOwner;

    // Parent Continent.
    Continent* continent;
public:
    // Counter for number of armies that current player owns in this country.
    int numberOfArmies;

    Territory(string name);
    Territory(Territory* territory);
    ~Territory();

    friend ostream& operator<<(ostream& stream, const Territory* territory);

    void operator=(const Territory* territory);

    /**
     * Changes the ownership of the country to a certain Player.
     * We expect the country to be removed from the previous Player's list
     * and added to the current Player's list.
     *
     * @param player Player to make owner of.
     */
    void set(Player* player);

    /**
     * Sets parent Continent.
     *
     * @param continent Continent to add.
     */
    void connect(Continent* continent);

    /**
     * Validates that node is connected.
     *
     * @return Node is connected.
     */
    bool validate();
};

ostream& operator<<(ostream& stream, const Territory* territory);