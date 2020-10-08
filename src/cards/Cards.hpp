#pragma once

#include <Player.hpp>

using std::vector;

class Card;

/**
 * Enumerator for card types.
 */
enum CardType {
    AIRLIFT,
    BLOCKADE,
    BOMB,
    DIPLOMACY,
    REINFORCEMENT,
    SPY
};


const int MAX_DECK_SIZE = 52;

class Deck {
private:
    vector<Card*> deck;
public:
    Deck();

    Card* draw(Player* player);
};



/**
 * A Risk card.
 */
class Card {
private:
    // CardType for card.
    CardType* cardType;
public:
    Card(const CardType& cardType);
    /**
     * Performs the card action on the player.
     * Removes card from player hand.
     * Places card back in deck.
     * @param player Player to perform card action on (and remove card from hand).
     * @param deck Deck to place card back into.
     */
    void play(Player* player, Deck* deck);
};



const int MAX_HAND_SIZE = 5;

class Hand {
private:
    Card cards[MAX_HAND_SIZE];
public:
    Hand(const Deck& deck);

    void addCard(Card* card);
};