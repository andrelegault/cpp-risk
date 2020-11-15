#include <iostream>
#include <assert.h>
#include "Cards.hpp"

using namespace std;

/**
 * Creates a deck of cards filled with all variants of cards.
 * Creates a hand using the deck to get cards from.
 * Plays the cards from hand.
 */
int main() {
    Deck deck(10);
    assert(deck.getLength() == 10);

    Player mockPlayer(deck);

    const Hand* hand = mockPlayer.hand;
    assert(hand->getLength() == 5);
    assert(deck.getLength() == 5);

    Deck deck2(deck);

    cout << deck2.getLength() << endl;

    assert(deck2.getAtIndex(0) != deck.getAtIndex(0));

    deck.draw(mockPlayer);
    assert(deck.getLength() == 4);
    assert(hand->getLength() == 6);

    Card* card = hand->getAtIndex(0);
    card->play(mockPlayer);
    assert(mockPlayer.orders->getLength() == 1);

    return 0;
}
