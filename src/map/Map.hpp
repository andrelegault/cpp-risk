#pragma once

#include <string>
#include <vector>
#include <Continent.hpp>
#include <Territory.hpp>
#include <Border.hpp>

using namespace std;

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