#include <string>
#include <vector>
#include <Border.h>
#include <Player.h>

using namespace std;

/**
 * Node for the Map graph.
 */
class Territory {
    private:
        // The name of the country as defined in the Conquer Map file.
        string name;
        // Current player that owns this country.
        Player* playerOwner;
        // Collection of Border edges.
        vector<Border*> borders;
    
    public:
        // Counter for number of armies that current player owns in this country.
        int number_of_armies;

        Territory(string name);

        /**
         * Inserts the Border edge reference.
         * We expect the border to include self.
         * 
         * @param border Border to attach to self.
         */
        void addBorder(Border* border);
        /**
         * Changes the ownership of the country to a certain Player.
         * We expect the country to be removed from the previous Player's list 
         * and added to the current Player's list.
         * 
         * @param player Player to make owner of.
         */
        void setOwnerPlayer(Player* player);
};