#pragma once

#include <vector>
#include <Continent.hpp>
#include <Territory.hpp>
#include <Border.hpp>

using namespace std;

class Continent;

/**
 * Bidirectional Graph collecting Territory nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and territories should be handle by the subgroups.
 */
class Map {
    private:
        // Collection of continents.
        vector<Continent*> continents;

    public:
        Map();
        ~Map();

        friend ostream& operator<<(ostream& stream, const Map* map);

        /**
         * Adds a continent to the map.
         * @param continent Continent to insert.
         */
        void add(Continent* continent);

        /**
         * Removes a continent to the map.
         * @param continent Continent to remove.
         */
        void remove(Continent* continent);

        /**
         * Method that checks if: 
         * 1) The map is a connected graph
         * 2) Continents are connected subgraphs
         * 3) Each territory belongs to one contient.
         */  
        bool validate();
};

ostream& operator<<(ostream& stream, const Map* map);