#ifndef Player_H
#define Player_H
#endif
#ifndef Deck_H
#define Deck_H
#endif
#include <CardType.h>

/**
 * A Risk card.
 */
class Card {
    private:
        // CardType for card.
        CardType* cardType;
    public:
        /**
         * Performs the card action on the player.
         * Removes card from player hand.
         * Places card back in deck.
         * @param player Player to perform card action on (and remove card from hand).
         * @param deck Deck to place card back into.
         */
        void play(Player* player, Deck* deck);
};