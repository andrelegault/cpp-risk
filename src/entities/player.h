#include <vector>

#ifndef country_H
#define country_H
#endif
#include <dice.h>
#include <hand.h>

using namespace std;

/**
 * A Risk player.
 */
class Player {
    private:
        // The countries owned by the players.
        vector<Country> countries;
        // The dice helper for the Player.
        Dice dice;
        // The card collection for the Player.
        Hand hand;
    public:
        /**
         * The reinforce phase of the Player's turn.
         */
        void reinforce();
        /**
         * The attack phase of the Player's turn.
         */
        void attack();
        /**
         * The fortify phase of the Player's turn.
         */
        void foritfy();
};
