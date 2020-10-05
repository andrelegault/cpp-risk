#pragma once

#include <vector>
#include <string>
#include <Continent.fwd.hpp>
#include <Map.fwd.hpp>
#include <Territory.hpp>
#include <Border.hpp>
#include <MapNode.hpp>
#include <Map.hpp>

using namespace std;

/**
 * A Node Subgroup for the Map graph.
 */
class Continent : public MapNode {
private:
    // Territories inside continent subgroup.
    vector<Territory*> territories;
    // Map that Continent is part of.
    Map* map;

public:
    Continent(string name);
    Continent(Continent* continent);
    ~Continent();

    friend ostream& operator<<(ostream& stream, const Continent* continent);

    void operator=(const Continent* continent);

    /**
     * Adds a territory to the territories list.
     * We expect the territory to stay in self for the duration of the game.
     *
     * @param territory Territory to add.
     */
    void connect(Territory* territory);

    /**
     * Sets parent map.
     * 
     * @param map Parent to set to.
     */
    void connect(Map* map);

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