#include "Cards.hpp"

string cardTypeToString(CardType cardType) {
    switch (cardType) {
    case 0: return "Airlift";
    case 1: return "Bomb";
    case 2: return "Blockade";
    case 3: return "Diplomacy"; 
    case 4: return "Reinforcement";
    default: return "";
    }
}

Deck::~Deck() {
    for (Card* card : cards) {
        delete card;
        card = nullptr;
    }

    cards.clear();
}

Deck::Deck(int size) {
    int variantCount = size / CardType::LENGTH;

    for (int i = 0; i < CardType::LENGTH; i++) {
        for(int j = 0; j < variantCount; j++) {
            this->addCard(new Card(new CardType((CardType) i), this));
        }
    }

    for (int i = 0; i < size - variantCount * CardType::LENGTH; ++i) {
        int type = rand() % CardType::LENGTH;

        this->addCard(new Card(new CardType((CardType)type), this));
    }

    cout << size << " -> " << this->cards.size() << endl;
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
    cards.insert(cards.begin() + (int) (rand() % (this->cards.size() + 1)), card);
}

ostream& operator<<(ostream& stream, const Deck& deck) {
    stream << "Deck (" << deck.cards.size() << " cards)" << endl;

    for (Card* card : deck.cards) {
        stream << "\t" << *card << endl;
    }

    return stream;
}

void Deck::draw(Player& player, int count) {
    player.hand->addCard(this->cards.back());

    this->cards.pop_back();
}

int Deck::getLength() const {
    return cards.size();
}

Card* Deck::getAtIndex(int index) {
    return cards.at(index);
}

Card::Card(const CardType* cardType, Deck* deck) : cardType(cardType), deck(deck) { }

Card::~Card() {
    delete cardType;
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

void Card::play(Player& player) {
    cout << "Playing card " << *this << endl;

    Order* order = nullptr;
    // switch (*(cardType)) {
    // case 0: order = new Bomb(); break;
    // case 1: order = new Airlift(); break;
    // case 2: order = new Blockade(); break;
    // case 3: order = new Negotiate(); break;
    // case 4: order = new Advance(); break;
    // case 5: order = new Deploy(); break;
    // }

    player.addOrder(order);
    player.hand->removeCard(this);
    this->deck->addCard(this);
}

ostream& operator<<(ostream& stream, const Card& card) {
    stream << cardTypeToString(*(card.cardType));
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
}

Hand::Hand(Deck& deck) {
    for (int i = 0; i < MAX_HAND_SIZE; ++i) {
        this->hand.push_back(deck.cards.back());
        deck.cards.pop_back();
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

    if (cardIt != hand.end()) hand.erase(cardIt);
}

vector<Card*> Hand::getCards() const {
    return this->hand;
}