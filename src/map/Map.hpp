#pragma once

#include <string>
#include <vector>

#include <Continent.hpp>
#include <Territory.hpp>
#include <Border.hpp>

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
 * Bidirectional Graph collecting Territory nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and territories should be handle by the subgroups.
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

    friend ostream& operator<<(ostream& stream, const Map* map);

    void operator=(const Map* map);

    /**
     * Adds a continent to the map.
     * @param continent Continent to insert.
     */
    void connect(Continent* continent);

    /**
     * Removes a continent to the map.
     * @param continent Continent to remove.
     */
    void remove(Continent* continent);

    /**
     * Returns list of all Continents.
     */
    vector<Continent*> getContinents();

    /**
     * Returns list of all Territories.
     */
    vector<Territory*> getTerritories();

    /**
     * Method that checks if:
     * 1) The map is a connected graph
     * 2) Continents are connected subgraphs
     * 3) Each territory belongs to one contient.
     */
    bool validate();
};

ostream& operator<<(ostream& stream, const Map* map);