#include "Player.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

Player::Player() {
    cout << "Player created" << endl;

}

//TODO wait for ALL OTHER CLASSES used to have their deep copy constructors implemented

Player::Player(const Player& player){
    this->orders = new OrdersList(player->orders);

    vector<Territory*> newTerList;
    for(int i = 0; i< this->territories.size(); i++){
        Territory* newTer = new Territory(this->territories);
        newTerList.push_back(newTer);
    }
    this->territories = newTerList;

    
    //this->hand = new Hand(player->hand);
}

//TODO delete hand when implemented
Player::~Player() {
    cout << "Player is being deleted" << endl;
    delete orders;
    orders = NULL;

    for(int i = 0; i< territories.size(); i++){
        delete territories.at(i);
        territories.at(i) = NULL;
    }

    /*
    delete hand;
    hand = NULL;
    */
}

//TODO get the name of the territory??
void Player::addTerritory(Territory* territory) {
    territories.push_back(territory);
    cout << "Added territory " << territory->getName() << " to the list" << endl;
}

//TODO test this
void Player::removeTerritory(Territory* territory) {
    auto t = find(this->territories.begin(), this->territories.end(), territory);
    if (t != this->territories.end()) this->territories.erase(t);
    cout << "Territory " << territory->getName() << " deleted" << endl;
}



//TODO: implement toDefend
vector<Territory*> Player::toDefend() {

}

//TODO: implement toAttack
vector<Territory*> Player::toAttack() {

}

void Player::issueOrder() {
    bool ok = true;
    Order* order;
    while (true) {
        int num;
        cout << "What type of Order would you like to create?" << endl;
        cout << " 1. Deploy\n 2. Advance\n 3. Bomb\n 4. Blockade\n 5. Airlift\n 6. Negotiate" << endl;
        cin >> num;
        if (num < 1 || num > 6) {
            cout << "Not a valid choice" << endl;
            continue;
        }
        if (num == 1) {
            order = new Deploy;
            cout << "Deploy order added to the Order List" << endl;
        }
        else if (num == 2) {
            order = new Advance;
            cout << "Advance order added to the Order List" << endl;
        }
        else if (num == 3) {
            order = new Bomb;
            cout << "Advance order added to the Order List" << endl;
        }
        else if (num == 4) {
            order = new Blockade;
            cout << "Blockade order added to the Order List" << endl;
        }
        else if (num == 5) {
            order = new Airlift;
            cout << "Airlift order added to the Order List" << endl;
        }
        else if (num == 6) {
            order = new Negotiate;
            cout << "Negotiate order added to the Order List" << endl;
        }
        break;
    }
    orders->addOrder(order);
}

std::ostream& operator<<(std::ostream& strm, const Player& player) {
    return strm << "Player object" ;
}

void Player::printTerritories(){
    cout << "Player's territories are:" << endl;
    for(int i=0; i<territories.size(); i++){
       cout << territories.at(i)->getName() << endl;
    }
}


/*
string Player::getName(){
    return name;
}
*/



