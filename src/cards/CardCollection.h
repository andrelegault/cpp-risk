#include <vector>

#include <Card.h>

using namespace std;

/**
 * An abstract representation of a card collection.
 */
class CardCollection {
    protected:
        // The raw Card collection.
        vector<Card*> cards;
};