#include "Player.hpp"

int Player::count = 0;

//set armies to 0 by default
Player::Player() : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand()), armies(0) {}

Player::Player(string name) : name(name), orders(new OrdersList()), hand(nullptr), armies(0) {}

Player::Player(Deck* deck) : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand(deck)), armies(0) {}

Player::Player(const Player& player) : name(player.name), orders(new OrdersList(*(player.orders))) {
    for (Territory* t : player.territories) {
        this->territories.push_back(new Territory(*t));
    }
}

Player::~Player() {
    delete orders;
    orders = nullptr;

    for (Territory* territory : territories) {
        delete territory;
        territory = nullptr;
    }

    territories.clear();

    delete hand;
    hand = nullptr;
}

void Player::addTerritory(Territory* territory) {
    this->territories.push_back(territory);

    territory->setPlayerOwner(this);
}


void Player::removeTerritory(Territory* territory) {
    auto t = getTerritory(territory);
    if (t != this->territories.end()) {
        territories.erase(t);
    }
}

vector<Territory*>::iterator Player::getTerritory(Territory* territory) {
    return find(territories.begin(), territories.end(), territory);
}

vector<Territory*> Player::toDefend() {
    return this->getTerritories();
}

int Player::getNumTerritories() const {
    return territories.size();
}

vector<Territory*> Player::toAttack() {
    vector<Territory*> enemyTerritories;

    for (auto territory : this->territories) {
        for (auto border : territory->getBorders()) {
            Territory* neighbor = (Territory*)border->getOther(territory);

            if (std::find(this->territories.begin(), this->territories.end(), neighbor) != this->territories.end()) {
                enemyTerritories.push_back(neighbor);
            }
        }
    }

    return enemyTerritories;
}

vector<Territory*> Player::getNeighbourTerritories(Territory* territory) {
    vector<Territory*> territories;

    for (auto border : territory->getBorders()) {
        Territory* otherTerritory = (Territory*)border->getOther(territory);

        if (otherTerritory->getOwner() == this) {
            territories.push_back(otherTerritory);
        }
    }

    return territories;
}

void Player::issueOrder() {
    vector<Territory*> territories = this->toDefend();

    std::shuffle(territories.begin(), territories.end(), std::random_device{});

    int roundRobin = 0;

    while (this->armies > 0) {
        int armyCount = rand() % (this->armies + 1);

        if (territories.size() > 0) {
            this->armies -= armyCount;

            this->addOrder(new Deploy(this, territories.at(roundRobin++ % static_cast<int>(territories.size())), armyCount));
        }
    }

    int duration = rand() % 10;

    for (int i = 0; i < duration; i++) {
        Territory* source = nullptr;

        if (rand() % 2 == 1 && this->toDefend().size() > 0) {
            source = this->toDefend().at(rand() % static_cast<int>(this->toDefend().size()));
        }
        else if (this->toAttack().size() > 0) {
            source = this->toAttack().at(rand() % static_cast<int>(this->toAttack().size()));
        }

        if (source != nullptr && source->numberOfArmies > 0) {
            vector<Territory*> neighbours = this->getNeighbourTerritories(source);

            if (neighbours.size() > 0) {
                Territory* target = neighbours.at(rand() % static_cast<int>(neighbours.size()));

                int armyCount = rand() % (static_cast<int>(source->numberOfArmies) + 1);

                this->addOrder(new Advance(this, source, target, armyCount));
            }
        }
    }

    if (this->hand->getLength() > 0) {
        this->hand->getCards().back()->play(*this);
    }

    this->hand->draw();

    this->notify();
}

void Player::addOrder(Order* order) {
    orders->add(order);
}

void Player::removeOrder(Order* order) {
    orders->remove(order);
}

int Player::remainingOrders() const {
    return orders->getLength();
}

ostream& operator<<(ostream& strm, const Player& player) {
    strm << "Player's name: " << player.name << ", Territories: [";

    for (int i = 0; i < player.territories.size(); i++) {
        if (i != player.territories.size() - 1) {
            strm << player.territories.at(i)->getName() + ", ";
        }
        else strm << player.territories.at(i)->getName();
    }

    strm << "]" << endl;

    return strm;
}

Player& Player::operator=(const Player& other) {
    if (&other == this) {
        return *this;
    }
    else {
        territories = other.territories;
        name = other.name;
        return *this;
    }
}

string Player::getName() const {
    return name;
}

void Player::addArmies(const int newArmies) {
    this->armies += newArmies;
}

vector<Territory*> Player::getTerritories() const {
    return this->territories;
}

int Player::getArmies() {
    return armies;
}

Order* Player::getNextOrder(const int wantedPriority) const {
    return orders->next(wantedPriority);
}
