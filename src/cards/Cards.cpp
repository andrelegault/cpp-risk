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
        for (int j = 0; j < variantCount; j++) {
            this->addCard(new Card(new CardType((CardType)i), this));
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
        cards.push_back(new Card(*card));
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
    if (this->cards.size() > 0) {
        int index = rand() % (this->cards.size() + 1);

        if (index >= cards.size()) {
            cards.push_back(card);
        }
        else {
            cards.insert(cards.begin() + index, card);
        }
    }
    else {
        cards.push_back(card);
    }
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

void Card::play(Player* player, Player* targetPlayer, Territory* source, Territory* targetTerritory, int armyCount) {
    Order* order = nullptr;

    switch (*(this->cardType)) {
    case CardType::AIRLIFT: order = new Airlift(player, source, targetTerritory, armyCount); break;
    case CardType::BOMB: order = new Bomb(player, targetTerritory); break;
    case CardType::BLOCKADE: order = new Blockade(player, source); break;
    case CardType::DIPLOMACY: order = new Negotiate(player, targetPlayer); break;
    case CardType::REINFORCEMENT: order = new Deploy(player, source, 5); break;
    }

    if (order) {
        player->addOrder(order);
    }

    player->hand->removeCard(this);

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

Hand::Hand(Deck* deck) : deck(deck) {
    for (int i = 0; i < 5; ++i) {
        this->draw();
    }
}

Hand::Hand(const Hand& other) {
    for (Card* card : other.hand) {
        hand.push_back(new Card(*card));
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

void Hand::draw() {
    this->hand.push_back(this->deck->cards.back());
    this->deck->cards.pop_back();
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