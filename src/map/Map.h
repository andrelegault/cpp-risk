#include <vector>
#include <Continent.h>
#include <Territory.h>
#include <Border.h>

using namespace std;

/**
 * Bidirectional Graph collecting Territory nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and territories should be handle by the subgroups.
 */
class Map {
    private:
        // Collection of territories. (Up for discussion)
        vector<Territory*> territories;
        // Collection of borders. (Up for discussion)
        vector<Border*> borders;
        // Collection of continents.
        vector<Continent*> continents;

    public:
        /**
         * Adds a territory to the map.
         * @param territory Territory to insert.
         */
        void addTerritory(Territory* territory);
         /**
         * Adds a continent to the map.
         * @param continent Continent to insert.
         */
        void addContinent(Continent* continent);
        /**
         * Adds a border to the map.
         * @param border Border to insert.
         */
        void addBorder(Border* border);
        /**
         * Method that checks if: 
         * 1) The map is a connected graph
         * 2) Continents are connected subgraphs
         * 3) Each territory belongs to one contient.
         */  
        bool validate();
};