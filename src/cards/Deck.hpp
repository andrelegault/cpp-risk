#pragma once

#include <CardCollection.hpp>
#include <Hand.hpp>

/**
 * A collection of card that can be drawn from.
 */
class Deck : public CardCollection {
private:
    // Size of the deck. Should start with as many cards as there are countries.
    int size;
public:
    Deck(int size);

    /**
     * Draws a random card from the deck and place it in Hand.
     *
     * @param hand Hand to place Card into.
     */
    void draw(Hand* hand);
};