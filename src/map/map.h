#include <vector>
#include <continent.h>
#include <country.h>
#include <border.h>

using namespace std;

/**
 * Bidirectional Graph collecting Country nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and countries should be handle by the subgroups.
 */
class Map {
    private:
        // Collection of countries. (Up for discussion)
        vector<Country> countries;
        // Collection of borders. (Up for discussion)
        vector<Border> borders;
        // Collection of continents.
        vector<Continent> continents;

    public:
        /**
         * Adds a country to the map.
         * @param country Country to insert.
         */
        void add_country(Country country);
         /**
         * Adds a continent to the map.
         * @param continent Continent to insert.
         */
        void add_continent(Continent continent);
        /**
         * Adds a border to the map.
         * @param border Border to insert.
         */
        void add_border(Border border);  
};