#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include <random>
#include <cassert>

#include "Cards.fwd.hpp"
#include "Player.hpp"

using std::vector;
using std::ostream;
using std::cout;
using std::string;

/**
 * Enumerator for card types.
 */
enum CardType {
    AIRLIFT,
    BOMB,
    BLOCKADE,
    DIPLOMACY,
    REINFORCEMENT,
    LENGTH
};

string cardTypeToString(CardType cardType);

/**
 * Represents a deck containing cards.
 */
class Deck {
public:
    /**
     * Default parameterized constructor.
     * @param size Number of cards to add to the deck.
     */
    Deck(int size = DEFAULT_DECK_SIZE);

    /**
     * Destructor.
     */
    ~Deck();

    /**
     * Copy constructor.
     * @param other Reference to object to copy.
     */
    Deck(const Deck& other);

    /**
     * Assignment operator.
     * @param other Reference to deck object used for assignment.
     */
    Deck& operator=(const Deck& other);

    /**
     * Stream insertion operator.
     * @param stream Stream object to output to.
     * @param deck Deck object to represent.
     */
    friend ostream& operator<<(ostream& stream, const Deck& deck);

    /**
     * Adds a card to the deck.
     * @param card Card to add to the deck.
     */
    void addCard(Card* const card);

    /**
     * Gets a card at the specified index.
     * @param index Index of the card.
     * @return Pointer to card object at specified index.
     */
    Card* getAtIndex(int index);

    /**
     * Gets the number of cards in the deck.
     * @return Number of cards in the deck.
     */
    int getLength() const;

    /**
     * Takes a random card from the deck and places it in hand.
     *
     * @param hand Hand to draw with.
     */
    void draw(Hand* hand);

    /// Used to make it have access to cards.
    friend Hand;

    /// Used to make it have access to cards.
    friend Player;
private:
    static const int DEFAULT_DECK_SIZE = 100;
    vector<Card*> cards;
};

/**
 * A Risk card.
 */
class Card {
public:
    // Destructor.
    ~Card();

    // CardType for card.
    const CardType* cardType;

    /**
     * Pamaraterized constructor.
     * @param cardType Type of card.
     * @param deck Parent deck.
     */
    Card(const CardType* cardType, Deck* deck);

    /**
     * Copy constructor.
     * @param other Card object to copy.
     */
    Card(const Card& other);

    /**
     * Assignment operator.
     * @param other Reference to Card object to assign to.
     */
    Card& operator=(const Card& other);

    /**
     * Stream insertion operator.
     * @param stream Stream object to output to.
     * @param card Card object to represent.
     */
    friend ostream& operator<<(ostream& stream, const Card& card);

    /**
     * 1. Creates an order
     * 2. Adds it to the player's list of orders.
     * 3. Removes card from player hand.
     * 4. Places card back in deck.
     * @param player Player to perform card action on (and remove card from hand).
     */
    void play(Player* p1, Player* targetPlayer = nullptr, Territory* source = nullptr, Territory* targetTerritory = nullptr, int armyCount = -1);
private:
    Deck* deck;
};


/**
 * Represents a hand, which is a subset of deck.
 */
class Hand {
public:
    // Default constructor.
    Hand();

    // Destructor.
    ~Hand();

    /**
     * Parameterized constructor.
     * @param deck Deck to draw cards from.
     */
    Hand(Deck* deck);

    /**
     * Copy constructor.
     * @param other Hand object to copy.
     */
    Hand(const Hand& other);

    /**
     * Stream insertion operator.
     * @param stream Stream object to output to.
     * @param card hand object to represent.
     */
    friend ostream& operator<<(ostream& stream, const Hand& hand);

    /**
     * Assignment operator.
     * @param other Reference to Hand object to assign to.
     */
    Hand& operator=(const Hand& other);

    /**
     * Gets a card at an index in the hand.
     * @param index Which card to return.
     * @return Pointer to card object at specified index.
     */
    Card* getAtIndex(int index) const;

    /**
     * Gets the number of cards in this hand.
     * @return Number of cards.
     */
    int getLength() const;

    /**
     * Adds a card to the hand.
     * @param card Card to add to the hand.
     */
    void addCard(Card* const card);

    /**
     * Removes a card from the hand.
     * @param card Card to remove.
     */
    void removeCard(Card* card);

    /**
     * Returns raw list of cards.
     */
    vector<Card*> getCards() const;

    /**
     * Draws a card from parent deck.
     */
    void draw();
private:
    // Holds the card pointers.
    vector<Card*> hand;

    // Parent deck.
    Deck* deck;
};
