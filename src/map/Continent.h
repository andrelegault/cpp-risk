#include <vector>
#include <string>
#include <Territory.h>

using namespace std;

/**
 * A Node Subgroup for the Map graph.
 */
class Continent {
    private:
        // Name of continent as defined in the Conquer Map.
        string name;
        // Territories inside continent subgroup.
        vector<Territory*> territories;
    
    public:
        Continent(string);

        /**
         * Adds a territory to the territories list.
         * We expect the territory to stay in self for the duration of the game.
         * 
         * @param territory Territory to add.
         */
        void addTerritory(Territory* territory);
        /**
         * Extension of the add_territory method.
         * This could be useful to make one-line insertions.
         * 
         * @param territories List of territories to add.
         */
        void addTerritories(vector<Territory*> territories);
};