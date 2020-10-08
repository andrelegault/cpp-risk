#pragma once

#include <Cards.fwd.hpp>
#include <Player.hpp>
#include <vector>
#include <stdlib.h>

using std::vector;

/**
 * Enumerator for card types.
 */
enum CardType {
    BOMB,
    AIRLIFT,
    BLOCKADE,
    DIPLOMACY,
    REINFORCEMENT,
    SPY
};


/**
 * Represents a deck containing cards.
 */
class Deck {
private:
    vector<Card*> deck;
public:
    Deck(int size = 20);

    ~Deck();

    /*
     * 1. Takes a card at random from the deck
     * 2. Places it in the player's hand.
     * @param player Player whose hand will be modified to include the card drawn from the deck.
     */
    void draw(Player* player);
};



/**
 * A Risk card.
 */
class Card {
private:
    // CardType for card.
    const CardType* cardType;
public:
    // Destructor.
    ~Card();

    Card(const CardType* cardType);

    /**
     * 1. Creates an order
     * 2. Adds it to the player's list of orders.
     * 3. Removes card from player hand.
     * 4. Places card back in deck.
     * @param player Player to perform card action on (and remove card from hand).
     * @param deck Deck to place card back into.
     */
    void play(Player* player, Deck* deck);
};



/**
 * Represents a hand, which is a subset of deck.
 */
class Hand {
private:
    vector<Card*> hand;
public:
    // Default constructor.
    Hand();

    // Constructor
    Hand(const Deck& deck);

    /**
     * Adds a card to the hand.
     * @param card Card to add to the hand.
     */
    void addCard(Card* card);
};