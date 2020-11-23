#include "Player.hpp"

int Player::count = -1;

Player::Player() : Player("Default Player", nullptr) {}

Player::Player(string name) : Player(name, nullptr) {}

Player::Player(Deck* deck, PlayerStrategy* initStrategy) : Player("Player " + to_string(++count), deck, initStrategy) {}

Player::Player(string name, Deck* deck, PlayerStrategy* initStrategy) : name(name), orders(new OrdersList()), hand(new Hand(deck)), armies(0), ps(initStrategy) {}

Player::Player(const Player& player) : name(player.name), orders(new OrdersList(*(player.orders))), deck(player.deck) {
    for (Territory* t : player.territories) {
        this->territories.push_back(new Territory(*t));
    }
    //should be okay since strategy doesn't have any class member objects?
    this->ps = player.ps;
}

Player& Player::operator=(const Player& other) {
    if (&other != this) {
        //TODO clear old territories list properly
        if (this->territories != nullptr) {
            delete territories;
        }
        for (Territory* t : other.territories) {
            this->territories.push_back(new Territory(*t));
        }
        deck = other.deck;
        name = other.name;
        ps = player.ps;
    }
    return *this;
}

Player::~Player() {
    delete orders;

    delete hand;

    delete ps;
}

void Player::setStrategy(PlayerStrategy *newStrategy) {
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
    return ps.toDefend();
    //return this->getTerritories();
}

int Player::getNumTerritories() const {
    return territories.size();
}

vector<Territory*> Player::toAttack() {
    
    return ps.toAttack();
    /*
    set<Territory*> enemyTerritories;

    for (auto territory : this->territories) {
        for (auto border : territory->getBorders()) {
            Territory* neighbour = (Territory*)border->getOther(territory);

            if (neighbour != nullptr && neighbour->getOwner() != this) {
                enemyTerritories.insert(neighbour);
            }
        }
    }

    return vector<Territory*>(enemyTerritories.begin(), enemyTerritories.end());
    */
}

void Player::issueOrder() {
    ps.issueOrder();
    /*
    vector<Territory*> defendTerritories = this->toDefend();

    std::shuffle(defendTerritories.begin(), defendTerritories.end(), std::random_device{});

    const int numberOfDef = defendTerritories.size();

    const int numberOfAtt = this->toAttack().size();

    int roundRobin = 0;

    int armiesAvailable = this->armies;

    // issue deployments to territories in a round robin fashion until no more armies
    while (armiesAvailable > 0) {
        int armyCount = Utils::getRandom(1, armiesAvailable);

        // the player has at least 1 territory to deploy to
        if (numberOfDef > 0) {
            armiesAvailable -= armyCount;

            Territory* target = defendTerritories.at(roundRobin++ % static_cast<int>(numberOfDef));

            this->addOrder(new Deploy(this, target, armyCount));
        }
        else {
            throw runtime_error("Player with no territory to defend should have been removed.");
        }
    }


    // create map to keep track of how many armies each player has issued
    map<Territory*, int> issuingState;
    for (auto territory : defendTerritories) {
        if (territory != nullptr) {
            issuingState[territory] = territory->getNumberOfArmies();
        }
    }

    // issue advance orders
    for (int i = 0; i < Advance::ADVANCE_ORDERS_ISSUED_PER_TURN; i++) {
        Territory* target;

        bool attacking = numberOfAtt > 0 && rand() % 2 == 0;

        if (attacking) {
            target = this->toAttack().at(Utils::getRandom(0, numberOfAtt - 1));
        }
        else {
            target = this->toDefend().at(Utils::getRandom(0, numberOfDef - 1));
        }

        if (target == nullptr) continue;

        vector<Territory*> neighbours = target->getPlayerBorderTerritories(this);

        if (neighbours.size() == 0) continue;

        Territory* source = neighbours[Utils::getRandom(0, neighbours.size() - 1)];

        if (source == nullptr) continue;

        int armyCount = Utils::getRandom(0, issuingState[source]);

        if (armyCount <= 0) continue;

        issuingState[source] -= armyCount;

        if (!attacking) issuingState[target] += armyCount;

        this->addOrder(new Advance(this, source, target, armyCount));
    }

    if (this->hand->getLength() > 0) {
        Card* toPlay = this->hand->getCards().back();
        CardType cardType = *(toPlay->cardType);
        Territory* randomSource = numberOfDef > 0 ? defendTerritories.at(Utils::getRandom(0, numberOfDef - 1)) : nullptr;
        Territory* randomTargetTerritory = numberOfAtt > 0 ? this->toAttack().at(Utils::getRandom(0, this->toAttack().size() - 1)) : nullptr;

        switch (cardType) {
        case CardType::BLOCKADE: toPlay->play(this, nullptr, randomSource); break;
        case CardType::BOMB: if (randomTargetTerritory != nullptr) toPlay->play(this, nullptr, nullptr, randomTargetTerritory); break;
        case CardType::DIPLOMACY:
            if (randomTargetTerritory != nullptr && randomTargetTerritory->getOwner() != &(Map::neutralP))
                toPlay->play(this, randomTargetTerritory == nullptr ? nullptr : randomTargetTerritory->getOwner(), nullptr);
            break;
        case CardType::REINFORCEMENT: toPlay->play(this, nullptr, randomSource, nullptr); break;
        case CardType::AIRLIFT:
            if (randomSource->getNumberOfArmies() > 0 && randomTargetTerritory != nullptr)
                toPlay->play(this, nullptr, randomSource, randomTargetTerritory, Utils::getRandom(1, randomSource->getNumberOfArmies()));
            break;
        }
    }

    this->notify();
    */
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
