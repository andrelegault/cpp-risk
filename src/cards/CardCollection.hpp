#pragma once

#include <vector>

#include <Card.hpp>

using namespace std;

class Card;

/**
 * An abstract representation of a card collection.
 */
class CardCollection {
protected:
    // The raw Card collection.
    vector<Card*> cards;
};