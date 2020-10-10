#include <iostream>
#include <Cards.hpp>
#include <assert.h>

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

    const Hand* const hand = mockPlayer.hand;
    assert(hand->getLength() == 5);

    deck.draw(mockPlayer);
    assert(deck.getLength() == 4);
    assert(hand->getLength() == 6);

    Card* card = hand->getAtIndex(0);
    card->play(mockPlayer, deck);
    assert(mockPlayer.orders->getLength() == 1);

    return 0;
}
