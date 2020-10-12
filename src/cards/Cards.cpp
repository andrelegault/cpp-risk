#include <Cards.hpp>
#include <algorithm>
#include <iostream>

using std::cout;


Deck::~Deck() {
    for (Card* card : cards) {
        delete card;
        card = nullptr;
    }
    cards.clear();
    cout << "Deck deleted!" << endl;
}

Deck::Deck(int size) {
    // set seed
    srand(time(NULL));

    for (unsigned int i = 0; i < size; ++i) {
        int type = rand() % 6;
        CardType* cardType = nullptr;
        switch (type) {
        case 0: cardType = new CardType(BOMB); break;
        case 1: cardType = new CardType(AIRLIFT); break;
        case 2: cardType = new CardType(BLOCKADE); break;
        case 3: cardType = new CardType(DIPLOMACY); break;
        case 4: cardType = new CardType(REINFORCEMENT); break;
        case 5: cardType = new CardType(SPY); break;
        }
        cards.push_back(new Card(cardType));
    }

    cout << "Created deck containing " << cards.size() << " cards!" << endl;
}

Deck::Deck(const Deck& other) {
    for (Card* card : other.cards) {
        Card* temp = new Card(*card);
        cards.push_back(temp);
    }
}

Deck& Deck::operator=(const Deck& other) {
    if (&other == this) {
        return *this;
    }
    else {
        cards = other.cards;
        return *this;
    }
}

void Deck::addCard(Card* const card) {
    cards.push_back(card);
}

ostream& operator<<(ostream& stream, const Deck& deck) {
    stream << "Deck (" << deck.cards.size() << " cards)" << endl;
    for (Card* card : deck.cards) {
        stream << "\t" << *card << endl;
    }
    return stream;
}

void Deck::draw(Player& player) {
    // Generate random number between 0 and deck size.
    int index = rand() % static_cast<int>(cards.size());

    // cout << "REMOVING FROM INDEX: " << index << " @ " << cards[index] << endl;

    player.hand->addCard(cards[index]);

    const auto o = find(cards.begin(), cards.end(), cards[index]);
    // Remove and get the card.
    cards.erase(o);
}

int Deck::getLength() const {
    return cards.size();
}

Card* Deck::getAtIndex(int index) {
    return cards.at(index);
}

Card::Card(const CardType* cardType) : cardType(cardType) { }

Card::~Card() {
    delete cardType;
    cardType = nullptr;
}

Card::Card(const Card& other) : cardType(new CardType(*(other.cardType))) { }

Card& Card::operator=(const Card& other) {
    if (&other == this) {
        return *this;
    }
    else {
        cardType = other.cardType;
        return *this;
    }
}

void Card::play(Player& player, Deck& deck) {
    cout << "Playing card " << *this << endl;

    Order* order = nullptr;
    switch (*(cardType)) {
    case 0: order = new Bomb(); break;
    case 1: order = new Airlift(); break;
    case 2: order = new Blockade(); break;
    case 3: order = new Negotiate(); break;
    case 4: order = new Advance(); break;
    case 5: order = new Deploy(); break;
    }

    player.addOrder(order);
    player.hand->removeCard(this);
    deck.addCard(this);
}

ostream& operator<<(ostream& stream, const Card& card) {
    switch (*(card.cardType)) {
    case 0: stream << "BOMB"; break;
    case 1: stream << "AIRLIFT"; break;
    case 2: stream << "BLOCKADE"; break;
    case 3: stream << "DIPLOMACY"; break;
    case 4: stream << "REINFORCEMENT"; break;
    case 5: stream << "SPY"; break;
    }
    stream << " @ " << &card;
    return stream;
}

Hand::Hand() { }

Hand::~Hand() {
    for (Card* c : hand) {
        delete c;
        c = nullptr;
    }
    hand.clear();
    cout << "Deleted hand!" << endl;
}

Hand::Hand(Deck& deck) {
    for (int i = 0; i < deck.getLength() || i < 5; ++i) {
        Card* card = deck.getAtIndex(i);
        auto c = find(deck.cards.begin(), deck.cards.end(), card);
        hand.push_back(card);
        deck.cards.erase(c);
    }
}

Hand::Hand(const Hand& other) {
    for (Card* card : other.hand) {
        Card* temp = new Card(*card);
        hand.push_back(temp);
    }
}

Hand& Hand::operator=(const Hand& other) {
    if (&other == this) {
        return *this;
    }
    else {
        hand = other.hand;
        return *this;
    }
}

Card* Hand::getAtIndex(int index) const {
    return hand.at(index);
}

ostream& operator<<(ostream& stream, const Hand& hand) {
    stream << "Hand (" << hand.hand.size() << " cards)" << endl;
    for (Card* card : hand.hand) {
        stream << "\t" << *card << endl;
    }
    return stream;
}

int Hand::getLength() const {
    return hand.size();
}

void Hand::addCard(Card* const card) {
    hand.push_back(card);
}

void Hand::removeCard(Card* card) {
    const auto cardIt = find(hand.begin(), hand.end(), card);
    if (cardIt != hand.end()) {
        hand.erase(cardIt);
    }
}
