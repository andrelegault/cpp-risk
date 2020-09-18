#pragma once

#include <vector>
#include <Order.h>
#include <Player.h>

using namespace std;

/**
 * Structure to hold orders and apply operations over.
 */
class OrdersList {
    private:
        // List of orders.
        vector<Order*> orders;
    public:
        /**
         * Checks if Order is valid.
         * @param order Order to check.
         * @return Order is valid.
         */
        bool validate(Order* order);

        /**
         * Validate then executes orders according to player's state and order definition.
         * @param player Player to execute orders on.
         */
        void execute(Player* player);
};