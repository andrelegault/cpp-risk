#include "Player.hpp"
#include <iostream>
#include <algorithm>

using namespace std;

Player::Player(){
    cout << "Player created" << endl;
    orders = new OrdersList();
}

//TODO look more into destructor
Player::~Player(){
     cout << "Player is being deleted" << endl;
}

//TODO get the name of the territory??
void Player::addTerritory(Territory* territory){
    territories.push_back(territory);
    cout << "Added territory to the list" << endl;
}

//TODO test this
void Player::removeTerritory(Territory* territory){
    auto t = find(this->territories.begin(), this->territories.end(), territory);
    if(t != this->territories.end()) this->territories.erase(t);
}

//TODO implement toDefend
vector<Territory*> Player::toDefend(){

}

//TODO implement toAttack
vector<Territory*> Player::toAttack(){

}

// TODO implement issueOrder
void Player::issueOrder(){
    //case for different type of order
}

//unrelated testing stuff
int Player::getNum(){
    return num;
}

string Player::getName(){
    return name;
}



