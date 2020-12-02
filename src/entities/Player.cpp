#include "Player.hpp"

int Player::count = -1;

Player::Player() : Player("Default Player", nullptr, nullptr) {}

Player::Player(string name) : Player(name, nullptr, nullptr) {}

Player::Player(Deck* deck, PlayerStrategy* initStrategy) : Player("Player " + to_string(++count), deck, initStrategy) {}

Player::Player(string name, Deck* deck, PlayerStrategy* initStrategy) : name(name), orders(new OrdersList()), hand(new Hand(deck)), armies(0), ps(initStrategy) {
    for (int i = 0; i < 10; i++) this->hand->draw();
}

Player::Player(const Player& player) : armies(player.armies), name(player.name), orders(new OrdersList(*(player.orders))), hand(player.hand) {
    for (Territory* t : player.territories) {
        this->territories.push_back(new Territory(*t));
    }
    //should be okay since strategy doesn't have any class member objects?
    this->ps = player.ps;
}

Player& Player::operator=(const Player& other) {
    if (&other != this) {
        for (Territory* t : other.territories) {
            this->territories.push_back(new Territory(*t));
        }

        this->hand = other.hand;
        this->name = other.name;
        this->ps = other.ps;
    }

    return *this;
}

Player::~Player() {
    delete orders;

    delete hand;

    delete ps;
}

void Player::setStrategy(PlayerStrategy* newStrategy) {
    delete this->ps;
    this->ps = newStrategy;
    cout << this->getName() << ": player strategy changed." << endl;
}

void Player::addTerritory(Territory* territory) {
    this->territories.push_back(territory);

    territory->setPlayerOwner(this);
}


void Player::removeTerritory(Territory* territory) {
    auto t = getTerritory(territory);

    if (t != this->territories.end()) territories.erase(t);
}

vector<Territory*>::iterator Player::getTerritory(Territory* territory) {
    return find(territories.begin(), territories.end(), territory);
}

vector<Territory*> Player::toDefend() {
    return this->ps->toDefend(this);
}

int Player::getNumTerritories() const {
    return territories.size();
}

vector<Territory*> Player::toAttack() {
    return this->ps->toAttack(this);
}

void Player::issueOrder() {
    this->ps->issueOrder(this);
    this->notify();
}

void Player::addOrder(Order* order) {
    if (order != nullptr) {
        orders->add(order);
    }
}

void Player::removeOrder(Order* order) {
    orders->remove(order);
}

int Player::remainingOrders() const {
    return orders->getLength();
}

ostream& operator<<(ostream& stream, const Player& player) {
    stream << "Player's name: " << player.name << ", Territories: [";

    for (int i = 0; i < player.territories.size(); i++) {
        if (i != player.territories.size() - 1) {
            stream << player.territories.at(i)->getName() + ", ";
        }
        else stream << player.territories.at(i)->getName();
    }

    stream << "], Orders: " << *player.orders << ", " << *player.hand << endl;

    return stream;
}

string Player::getName() const {
    return name;
}

vector<Territory*> Player::getTerritories() const {
    return this->territories;
}

Order* Player::getNextOrder(const int wantedPriority) const {
    return orders->next(wantedPriority);
}

PlayerStrategy& Player::getStrategy() const {
    return *(this->ps);
}