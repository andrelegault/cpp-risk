#include <Cards.hpp>
#include <algorithm>
#include <iostream>

using std::cout;

Deck::~Deck() {
    for (Card* card : deck) {
        delete card;
        card = nullptr;
    }
    deck.clear();
}

Deck::Deck(int size) {
    // set seed
    srand(time(NULL));

    for (unsigned int i = 0; i < size; ++i) {
        int type{ rand() % 6 };
        CardType* cardType;
        switch (type) {
        case 0: cardType = new CardType(BOMB); break;
        case 1: cardType = new CardType(AIRLIFT); break;
        case 2: cardType = new CardType(BLOCKADE); break;
        case 3: cardType = new CardType(DIPLOMACY); break;
        case 4: cardType = new CardType(REINFORCEMENT); break;
        case 5: cardType = new CardType(SPY); break;
        }
        deck.push_back(new Card(cardType));
    }

    cout << "Created deck containing " << deck.size() << " cards!" << endl;
}

void Deck::draw(Player* player) {
    // Generate random number between 0 and deck size.
    int index{ rand() % static_cast<int>(deck.size()) };

    // Remove and get the card.
    auto toRemove = deck.erase(deck.begin() + index);

    // Add it to the player's hand.
    player->hand->addCard(*toRemove);
}

Card::Card(const CardType* cardType) : cardType(cardType) { }

Card::~Card() {
    delete cardType;
    cardType = nullptr;
}

Hand::Hand() { }

Hand::Hand(const Deck& deck) { }

void Hand::addCard(Card* card) {
    hand.push_back(card);
}