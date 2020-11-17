#include "Player.hpp"

int Player::count = 0;

//set armies to 0 by default
Player::Player() : name("Player " + to_string(++count)), orders(new OrdersList()), hand(new Hand()), armies(0) {}

Player::Player(string name) : name(name), orders(new OrdersList()), hand(nullptr), armies(0) {}

Player::Player(Deck* deck) : name("Player " + to_string(++count)), orders(new OrdersList()), hand(new Hand(deck)), armies(0) {}

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
    set<Territory*> enemyTerritories;

    for (auto territory : this->territories) {
        for (auto border : territory->getBorders()) {
            Territory* neighbour = (Territory*)border->getOther(territory);

            if (neighbour->getOwner() != this) {
                enemyTerritories.insert(neighbour);
            }
        }
    }

    return vector<Territory*>(enemyTerritories.begin(), enemyTerritories.end());
}

void Player::issueOrder() {
    vector<Territory*> def = this->toDefend();

    std::shuffle(def.begin(), def.end(), std::random_device{});

    int roundRobin = 0;

    int armiesAvailable = this->armies;

    // for (auto territory : this->territories) {
    //     if (territory->numberOfArmies == 0 && armiesAvailable > 0) {
    //         armiesAvailable -= 1;
    //         this->addOrder(new Deploy(this, territory, 1));
    //     }
    // }

    // issue deployments to territories in a round robin fashion until no more armies
    while (armiesAvailable > 0) {
        int armyCount = Utils::getRandom(1, armiesAvailable);

        // the player has at least 1 territory to deploy to
        if (def.size() > 0) {
            armiesAvailable -= armyCount;

            this->addOrder(new Deploy(this, def.at(roundRobin++ % static_cast<int>(def.size())), armyCount));
        }
        else {
            throw runtime_error("should not happen, player should've been removed!");
        }
    }

    const int numberOfDef = def.size();
    const int numberOfAtt = this->toAttack().size();

    map<Territory*, int> issuingState;
    for (auto territory : def) {
        issuingState[territory] = territory->numberOfArmies;
    }

    // issue 10 advance orders
    for (auto territory : def) {
        if (issuingState.at(territory) > 0) {
            int rDef = Utils::getRandom(0, numberOfDef - 1);
            Territory* source = def.at(rDef);

            // there are enemy territories to attack
            bool attacking = numberOfAtt > 0 && rand() % 2 == 0 ? true : false;
            int rTarget = attacking ? Utils::getRandom(0, numberOfAtt - 1) : Utils::getRandom(0, numberOfDef - 1);
            Territory* target = attacking ? toAttack().at(rTarget) : def.at(rTarget);

            if (target != source) {
                int armyCount = Utils::getRandom(1, issuingState.at(territory));
                issuingState[source] -= armyCount;

                this->addOrder(new Advance(this, source, target, armyCount));
            }
        }
    }

    // for (int i = 0; i < 10; i++) {
    //     Territory* target = nullptr;

    //     if (rand() % 2 == 1 && this->toDefend().size() > 0) {
    //         target = this->toDefend().at(rand() % static_cast<int>(this->toDefend().size()));
    //     }
    //     else if (this->toAttack().size() > 0) {
    //         target = this->toAttack().at(rand() % static_cast<int>(this->toAttack().size()));
    //     }

    //     if (target != nullptr && target->numberOfArmies > 1) {
    //         int armyCount = rand() % static_cast<int>(target ->numberOfArmies);

    //         vector<Territory*> neighbours = target ->getPlayerBorderTerritories(this);

    //         if (neighbours.size() > 0) {
    //             Territory* source = neighbours.at(rand() % static_cast<int>(neighbours.size()));

    //             this->addOrder(new Advance(this, source, target, armyCount));
    //         }
    //     }
    // }

    if (this->hand->getLength() > 0) {
        Card* toPlay = this->hand->getCards().back();
        CardType cardType = *(toPlay->cardType);
        Territory* randomSource = numberOfDef > 0 ? def.at(Utils::getRandom(0, numberOfDef - 1)) : nullptr;
        Territory* randomTargetTerritory = numberOfAtt > 0 ? this->toAttack().at(Utils::getRandom(0, this->toAttack().size() - 1)) : nullptr;

        switch (cardType) {
        case CardType::BLOCKADE: toPlay->play(this, nullptr, randomSource); break;
        case CardType::BOMB: if (randomTargetTerritory != nullptr) toPlay->play(this, nullptr, nullptr, randomTargetTerritory); break;
        case CardType::DIPLOMACY: if (randomTargetTerritory != nullptr) toPlay->play(this, randomTargetTerritory == nullptr ? nullptr : randomTargetTerritory->getOwner(), nullptr); break;
        case CardType::REINFORCEMENT: toPlay->play(this, nullptr, randomSource, nullptr); break;
        case CardType::AIRLIFT:
            if (randomSource->numberOfArmies > 0 && randomTargetTerritory != nullptr)
                toPlay->play(this, nullptr, randomSource, randomTargetTerritory, randomSource == nullptr ? -1 : Utils::getRandom(1, randomSource->numberOfArmies));
            break;
        }
    }

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

vector<Territory*> Player::getTerritories() const {
    return this->territories;
}

Order* Player::getNextOrder(const int wantedPriority) const {
    return orders->next(wantedPriority);
}
