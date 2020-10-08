#include "Player.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

int Player::count = 0;

Player::Player() {
   
    
    name = "p" + to_string(count++);
    orders = new OrdersList();
    cout << "Player " <<  name << " created" << endl;

}

//TODO wait for other classes used to have their deep copy constructors implemented
Player::Player(const Player& player){
    /*
    name = "p" + to_string(count++);
    this->orders = new OrdersList(player.orders);

    vector<Territory*> newTerList;
    for(int i = 0; i< this->territories.size(); i++){
        Territory* newTer = new Territory(player.territories);
        newTerList.push_back(newTer);
    }
    this->territories = newTerList;
    */
    
    //this->hand = new Hand(player->hand);
    
}


//TODO delete hand when implemented
Player::~Player() {
    
    cout << "Player is being deleted" << endl;

    delete orders;
    orders = NULL;

    for(int i=0; i<territories.size(); i++){
        delete territories.at(i);
        territories.at(i) = NULL;
    }
    territories.clear();

    /*
    delete hand;
    hand = NULL;

    */
}


void Player::addTerritory(Territory* territory) {
    territories.push_back(territory);
    cout << "Added territory " << territory->getName() << " to the list" << endl;
}


void Player::removeTerritory(Territory* territory) {
    auto t = find(this->territories.begin(), this->territories.end(), territory);
    if (t != this->territories.end()) this->territories.erase(t);
    cout << "Territory " << territory->getName() << " deleted" << endl;
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

    cout << "List of territories to defend:" << endl;
    /*
    for(Territory* t : toDefend){
       cout << t->getName() << endl;
    }
    */

   for(int i=0; i<toDefend.size(); i++){
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

    cout << "List of territories to Attack:" << endl;
    
   for(int i=0; i<toAttack.size(); i++){
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
    cout << "order added to the Order List" << endl;
}

//TODO add actual usful info to this
std::ostream& operator<<(std::ostream& strm, const Player& player) {
    return strm << "Player object" ;
}

void Player::printTerritories(){
    cout << "Player's territories are:" << endl;
    for(int i=0; i<territories.size(); i++){
       cout << territories.at(i)->getName() << endl;
    }
}


string Player::getName(){
    return name;
}





