#include <Player.hpp>

int Player::count = 0;

//set armies to 0 by default
Player::Player() : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand()), armies(0) {
    cout << "Player " << name << " created" << endl;
}


Player::Player(const Player& player) : name("p" + to_string(count++)), orders(new OrdersList(*(player.orders))) {
    cout << "Player " << name << " created (copy constructor)" << endl;

    for (Territory* t : player.territories) {
        Territory* newTer = new Territory(*t);
        this->territories.push_back(newTer);
    }
}


Player::~Player() {
    cout << "Player is being deleted" << endl;

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


Player::Player(Deck& deck) :
    hand(new Hand(deck)),
    name("p" + to_string(count++)),
    orders(new OrdersList()) { }


void Player::addTerritory(Territory* territory) {
    territories.push_back(territory);
    cout << "Player " << name << ": Added territory " << territory->getName() << " to the Territory List" << endl;
}


void Player::removeTerritory(Territory* territory) {
    auto t = find(territories.begin(), territories.end(), territory);
    territories.erase(t);
    cout << "Player " << name << ": Removed territory " << territory->getName() << " from the Territory List " << name << endl;
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

        // TODO: The Deploy order should take the territory as parameter!
        orders->addOrder(new Deploy());
    }

    while (true) {
        switch (UI::ask("What you like to do?", { "Attack", "Defend", "End" })) {
        case 1:
            int territory = UI::ask("Which territory to attack?", territoriesToAttackStrings) - 1;

            // TODO: The Advance should take the territory.
            orders->addOrder(new Advance());

            break;
        case 2:
            int territory = UI::ask("Which territory to defend?", territoriesToDefendStrings) - 1;

            // TODO: The Advance should take the territory.
            orders->addOrder(new Advance());

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

    vector<CardType> cardTypeVector(cardTypeMap.begin(), cardTypeMap.end());
    vector<string> cardTypeStrings;

    for (auto cardType : cardTypeVector) {
        cardTypeStrings.push_back(cardTypeToString(cardType));
    }

    int cardIndex = UI::ask("Choose a card to play.", cardTypeStrings) - 1;

    Card* card = cardTypeMap[cardTypeVector[cardIndex]];

    // TODO: Find a way to map acces the GameEngine deck.
    // card->play(*this, DECK);
}

void Player::addOrder(Order* order) {
    orders->addOrder(order);
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
    armies += newArmies;
}


vector<Territory*> Player::getTerritories() const {
    return this->territories;
}

int Player::getArmies() {
    return armies;
}