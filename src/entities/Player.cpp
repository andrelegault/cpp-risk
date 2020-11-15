#include "Player.hpp"

int Player::count = 0;

//set armies to 0 by default
Player::Player() : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand()), armies(0) {}

Player::Player(Deck& deck) : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand(deck)), armies(0) {}

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

void Player::issueOrder() {
    vector<string> territoriesToDefendStrings;

    for (auto territory : this->toDefend()) {
        territoriesToDefendStrings.push_back(territory->getName());
    }

    vector<string> territoriesToAttackStrings;

    for (auto territory : this->toAttack()) {
        territoriesToAttackStrings.push_back(territory->getName());
    }

    while (this->armies > 0) {
        int territoryIndex = UI::ask("Select territory to deploy to.", territoriesToDefendStrings) - 1;

        int numberOfArmies = UI::range("Enter number of armies.", 0, this->armies);

        // TODO: use actual territory pointer
        orders->add(new Deploy(this, NULL));

        this->armies -= numberOfArmies;
    }

    while (true) {
        int territory;
        switch (UI::ask("What would you like to do?", { "Attack", "Defend", "End" })) {
        case 1:
            territory = UI::ask("Which territory to attack?", territoriesToAttackStrings) - 1;

            // TODO: The Advance should take the territory.
            // orders->add(new Advance(this));

            break;
        case 2:
            territory = UI::ask("Which territory to defend?", territoriesToDefendStrings) - 1;

            // TODO: The Advance should take the territory.
            // orders->add(new Advance(this));

            break;
        default:
            goto nextState;
        }
    }

nextState:;

    map<CardType, Card*> cardTypeMap;

    for (auto card : this->hand->getCards()) {
        cardTypeMap[*(card->cardType)] = card;
    }

    vector<CardType> cardTypeVector;

    std::transform(cardTypeMap.begin(), cardTypeMap.end(), std::back_inserter(cardTypeVector), [](auto& pair) { return pair.first; });

    vector<string> cardTypeStrings;

    for (auto cardType : cardTypeVector) {
        cardTypeStrings.push_back(cardTypeToString(cardType));
    }

    int cardIndex = UI::ask("Choose a card to play.", cardTypeStrings) - 1;

    Card* card = cardTypeMap[cardTypeVector[cardIndex]];

    card->play(*this);
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