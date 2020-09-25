#pragma once

#include <vector>
#include <string>
#include <Territory.hpp>
#include <Border.hpp>
#include <MapNode.hpp>

using namespace std;

/**
 * A Node Subgroup for the Map graph.
 */
class Continent : public MapNode {
private:
    // Territories inside continent subgroup.
    vector<Territory*> territories;

public:
    Continent(string name);
    Continent(Continent* continent);

    friend ostream& operator<<(ostream& stream, const Continent* continent);

    /**
     * Adds a territory to the territories list.
     * We expect the territory to stay in self for the duration of the game.
     *
     * @param territory Territory to add.
     */
    void add(Territory* territory);

    /**
     * Removes Territory from territories.
     *
     * @param territory Territory to remove.
     */
    void remove(Territory* territory);

    /**
     * Get territories.
     *
     * @return Territories.
     */
    vector<Territory*> getTerritories();

    /**
     * Checks if containing territories are connected.
     *
     * @return Are territories connected?
     */
    bool validate();
};

ostream& operator<<(ostream& stream, const Continent* continent);