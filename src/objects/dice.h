#include<map>
#include<vector>

using namespace std;

/**
 * A helper class for Player to handle dice rolls.
 */
class Dice {
    private:
        // Keeps track of the faces rolled on the dice.
        map<int, int> roll_count;
    public:
        /**
         * Rolls 1-3 6 sided dice into a sorted container.
         * Keeps track of the rolled faces in the roll_count map.
         * 
         * @param number_of_dice Number of dice to roll (1-3).
         * @return Sorted container of values rolled.
         */
        vector<int> roll(int number_of_dice);
};