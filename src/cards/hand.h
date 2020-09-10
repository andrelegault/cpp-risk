#include <card.h>
#include <card_collection.h>

/**
 * A card collection that can exchange cards used for Player.
 */
class Hand: public Card_Collection {
    public:
        /**
         * Exchanges a Card for a number of armies.
         * 
         * @param card The Card to exchange.
         * @return The number of armies that were exchanged.
         */
        int exchange(Card card);
};