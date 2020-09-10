#include <vector>

#include <card.h>

using namespace std;

/**
 * An abstract representation of a card collection.
 */
class Card_Collection {
    protected:
        // The raw Card collection.
        vector<Card> cards;
};