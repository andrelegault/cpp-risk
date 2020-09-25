#pragma once

#include <Player.hpp>
#include <Deck.hpp>
#include <CardType.hpp>

class Player;

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