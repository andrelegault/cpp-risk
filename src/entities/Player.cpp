#include <Player.hpp>
#include <iostream>
#include <algorithm>

int Player::count = 0;


Player::Player() : name("p" + to_string(count++)), orders(new OrdersList()), hand(new Hand()) {
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
    auto last = territories.begin() + (rand() % territories.size());

    cout << "Player " << name << "'s list of territories to defend: [";

    vector<Territory*> toDefend(territories.begin(), last);

    for (Territory* t : toDefend) {
        cout << t->getName() << ", ";
    }

    cout << "]" << endl;

    return toDefend;
}

int Player::getNumTerritories() const {
    return territories.size();
}

vector<Territory*> Player::toAttack() {
    auto last = territories.begin() + (rand() % territories.size());

    cout << "Player " << name << "'s list of territories to attack: [";

    vector<Territory*> toAttack(territories.begin(), last);

    for (Territory* t : toAttack) {
        cout << t->getName() << ", ";
    }

    cout << "]" << endl;

    return toAttack;
}

void Player::issueOrder() {
    Order* order = nullptr;
    while (true) {
        int num;
        cout << "What type of Order would you like to create?" << endl;
        cout << "1. Deploy" << endl << "2. Advance" << "3. Bomb" << endl << "4. Blockade" << endl << "5. Airlift" << endl << "6. Negotiate" << endl;
        cin >> num;
        if (num < 1 || num > 6) {
            cout << "Not a valid choice" << endl;
            continue;
        }
        if (num == 1) {
            order = new Deploy();
            cout << "Deploy order added to the Order List" << endl;
        }
        else if (num == 2) {
            order = new Advance();
            cout << "Advance order added to the Order List" << endl;
        }
        else if (num == 3) {
            order = new Bomb();
            cout << "Advance order added to the Order List" << endl;
        }
        else if (num == 4) {
            order = new Blockade();
            cout << "Blockade order added to the Order List" << endl;
        }
        else if (num == 5) {
            order = new Airlift();
            cout << "Airlift order added to the Order List" << endl;
        }
        else if (num == 6) {
            order = new Negotiate();
            cout << "Negotiate order added to the Order List" << endl;
        }
        break;
    }
    orders->addOrder(order);

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
    } else {
        territories = other.territories;
        name = other.name;
        return *this;
    }
}


string Player::getName() const {
    return name;
}
