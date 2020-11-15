#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iterator>
#include <Cards.fwd.hpp>
#include "Player.hpp"

using std::vector;
using std::ostream;

/**
 * Enumerator for card types.
 */
enum CardType {
    AIRLIFT,
    BOMB,
    BLOCKADE,
    DIPLOMACY,
    REINFORCEMENT
};

string cardTypeToString(CardType cardType);

/**
 * Represents a deck containing cards.
 */
class Deck {
private:
    vector<Card*> cards;
public:
    /**
     * Default parameterized constructor.
     * @param size Number of cards to add to the deck.
     */
    Deck(int size = 20);

    /**
     * Destructor.
     */
    ~Deck();

    /**
     * Copy constructor.
     * @param other Reference to object to copy.
     */
    Deck(const Deck& other);

    /// Used to make it have access to cards.
    friend Hand;

    /// Used to make it have access to cards.
    friend Player;

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

    /*
     * 1. Takes a card at random from the deck
     * 2. Places it in the player's hand.
     * @param player Player whose hand will be modified to include the card drawn from the deck.
     */
    void draw(Player& player);
};



/**
 * A Risk card.
 */
class Card {
private:
public:
    // Destructor.
    ~Card();

    // CardType for card.
    const CardType* cardType;

    /**
     * Pamaraterized constructor.
     * @param cardType Type of card.
     */
    Card(const CardType* cardType);

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
     * @param deck Deck to place card back into.
     */
    void play(Player& player, Deck& deck);
};



/**
 * Represents a hand, which is a subset of deck.
 */
class Hand {
private:
    // Holds the card pointers.
    vector<Card*> hand;
    const int MAX_HAND_SIZE = 5;
public:
    // Default constructor.
    Hand();
    // Destructor.
    ~Hand();

    /**
     * Parameterized constructor.
     * @param deck Deck to draw cards from.
     */
    Hand(Deck& deck);

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

    vector<Card*> getCards() const;
};
