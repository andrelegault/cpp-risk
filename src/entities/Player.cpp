#include <Player.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

int Player::count = 0;

Player::Player() : orders(new OrdersList()), hand(new Hand()) {
    name = "p" + to_string(count++);
    cout << "Player " << name << " created\n" << endl;
}

//TODO wait for other classes used to have their deep copy constructors implemented
Player::Player(const Player& player) {

    name = "p" + to_string(count++);
    cout << "Player " << name << " created\n" << endl;
    this->orders = new OrdersList(player.orders);


    vector<Territory*> newTerList;
    for (int i = 0; i < this->territories.size(); i++) {
        Territory* newTer = new Territory(player.territories[i]);
        newTerList.push_back(newTer);
    }
    this->territories = newTerList;

    //this->hand = new Hand(player->hand);

}


//TODO delete hand when implemented
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


void Player::addTerritory(Territory* territory) {
    territories.push_back(territory);
    cout << "\nPlayer " << name << ": Added territory " << territory->getName() << " to the Territory List" << endl;
}


void Player::removeTerritory(Territory* territory) {
    auto t = find(territories.begin(), territories.end(), territory);
    territories.erase(t);
    cout << "\nPlayer " << name << ": Removed territory " << territory->getName() << " from the Territory List " << name << endl;
}


vector<Territory*> Player::toDefend() {
    Territory* t1 = new Territory("toDefend1");
    Territory* t2 = new Territory("toDefend2");
    Territory* t3 = new Territory("toDefend3");
    Territory* t4 = new Territory("toDefend24");

    vector<Territory*> toDefend;

    toDefend.push_back(t1);
    toDefend.push_back(t2);
    toDefend.push_back(t3);
    toDefend.push_back(t4);

    cout << "\nPlayer " << name << ":List of territories to defend:" << endl;
    /*
    for(Territory* t : toDefend){
       cout << t->getName() << endl;
    }
    */

    for (int i = 0; i < toDefend.size(); i++) {
        cout << toDefend.at(i)->getName() << endl;
    }

    return toDefend;
}


vector<Territory*> Player::toAttack() {
    Territory* t1 = new Territory("toAttack1");
    Territory* t2 = new Territory("toAttack2");
    Territory* t3 = new Territory("toAttack3");
    Territory* t4 = new Territory("toAttack4");

    vector<Territory*> toAttack;

    toAttack.push_back(t1);
    toAttack.push_back(t2);
    toAttack.push_back(t3);
    toAttack.push_back(t4);

    cout << "\nPlayer " << name << ": List of territories to Attack:" << endl;

    for (int i = 0; i < toAttack.size(); i++) {
        cout << toAttack.at(i)->getName() << endl;
    }

    return toAttack;
}

void Player::issueOrder() {
    Order* order;
    while (true) {
        int num;
        cout << "What type of Order would you like to create?" << endl;
        cout << " 1. Deploy\n 2. Advance\n 3. Bomb\n 4. Blockade\n 5. Airlift\n 6. Negotiate" << endl;
        cin >> num;
        if (num < 1 || num > 6) {
            cout << "Not a valid choice\n" << endl;
            continue;
        }
        if (num == 1) {
            order = new Deploy();
            cout << "Deploy order added to the Order List\n" << endl;
        }
        else if (num == 2) {
            order = new Advance();
            cout << "Advance order added to the Order List\n" << endl;
        }
        else if (num == 3) {
            order = new Bomb();
            cout << "Advance order added to the Order List\n" << endl;
        }
        else if (num == 4) {
            order = new Blockade();
            cout << "Blockade order added to the Order List\n" << endl;
        }
        else if (num == 5) {
            order = new Airlift();
            cout << "Airlift order added to the Order List\n" << endl;
        }
        else if (num == 6) {
            order = new Negotiate();
            cout << "Negotiate order added to the Order List\n" << endl;
        }
        break;
    }
    orders->addOrder(order);

}

//TODO maybe figure out how to add orders to this
std::ostream& operator<<(std::ostream& strm, const Player& player) {
    strm << "\nPlayer's name: " << player.name << ", Territories: [";
    for (int i = 0; i < player.territories.size(); i++) {
        if (i != player.territories.size() - 1) {
            strm << player.territories.at(i)->getName() + ", ";
        }
        else strm << player.territories.at(i)->getName();
    }
    strm << "]\n";
    return strm;
}

string Player::printTerritoriesStr() {
    string toPrint = "Player's territories are:";
    for (int i = 0; i < territories.size(); i++) {
        toPrint = toPrint + territories.at(i)->getName() + "\n";
    }
    return toPrint;
}

void Player::printTerritories() const {
    cout << "Player's territories are:" << endl;
    for (int i = 0; i < territories.size(); i++) {
        cout << territories.at(i)->getName() << endl;
    }
}


string Player::getName() const {
    return name;
}
