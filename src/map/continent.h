#include <vector>
#include <string>
#include <country.h>

using namespace std;

/**
 * A Node Subgroup for the Map graph.
 */
class Continent {
    private:
        // Name of continent as defined in the Conquer Map.
        string name;
        // Countries inside continent subgroup.
        vector<Country> countries;
    
    public:
        Continent(string);

        /**
         * Adds a country to the countries list.
         * We expect the country to stay in self for the duration of the game.
         * 
         * @param country Country to add.
         */
        void add_country(Country country);
        /**
         * Extension of the add_country method.
         * This could be useful to make one-line insertions.
         * 
         * @param countries List of countries to add.
         */
        void add_countries(vector<Country> countries);
};