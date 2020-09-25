#pragma once

#include <vector>

#include <Territory.hpp>
#include <Hand.hpp>
#include <OrdersList.hpp>

using namespace std;

class OrdersList;

/**
 * A Risk player.
 */
class Player {
    private:
        // The territories owned by the players.
        vector<Territory*> territories;
        // The card collection for the Player.
        Hand hand;
        // Orders to apply.
        OrdersList orders;
    public:
        /**
         * @return A list of territories to defended.
         */
        vector<Territory*> toDefend();

        /**
         * @return A list of territories to attack.
         */
        vector<Territory*> toAttack();
        
        /**
         * Creates an Order and adds it to the list of orders.
         */
        void issueOrder();

        /**
         * Add territory to territories.
         */
        void addTerritory(Territory* territory);

        /**
         * Remove territory from territories.
         */
        void removeTerritory(Territory* territory);
};
