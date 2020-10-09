#pragma once

#include <string>
#include <vector>

#include <Map.fwd.hpp>
#include <Border.hpp>
#include <Player.hpp>


using namespace std;

/**
 * Abstracted node for bidirectional graphs.
 */
class MapNode {
protected:
    // The name as defined in the Conquer Map file.
    string name;
    // Collection of Border edges.
    vector<Border*> borders;
public:
    MapNode(string name);
    MapNode(MapNode* mapNode);
    ~MapNode();

    string getName();

    /**
     * Inserts the Border edge reference.
     * We expect the border to include self.
     *
     * @param border Border to attach to self.
     */
    void connect(Border* border);

    /**
     * Removes the Border edge reference.
     *
     * @param border Border to detach from self.
     */
    void remove(Border* border);

    /**
     * Connects other node to this node
     *
     * @param node Node to connect
     */
    void connect(MapNode* node);
};

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

    friend ostream& operator<<(ostream& stream, const Continent& continent);

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

ostream& operator<<(ostream& stream, const Continent& continent);


// Territory

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

    friend ostream& operator<<(ostream& stream, const Territory& territory);

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


/**
 * Bidirectional Graph collecting Territory nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and territories should be handled by the subgroups.
 */
class Map {
private:
    // Collection of continents.
    vector<Continent*> continents;

    // Map name.
    string name;

public:
    Map(string name);
    Map(Map* map);
    ~Map();

    friend ostream& operator<<(ostream& stream, const Map& map);

    void operator=(const Map* map);

    /**
     * Adds a continent to the map.
     * @param continent Continent to insert.
     */
    void connect(Continent* continent);

    /**
     * Removes a continent from the map.
     * @param continent Continent to remove.
     */
    void remove(Continent* continent);

    /**
     * Returns list of all Continents.
     */
    vector<Continent*> getContinents() const;

    /**
     * Returns list of all Territories.
     */
    vector<Territory*> getTerritories() const;

    /**
     * Method that checks if:
     * 1) The map is a connected graph
     * 2) Continents are connected subgraphs
     * 3) Each territory belongs to one contient.
     * @return The Map is valid.
     */
    bool validate();
};
