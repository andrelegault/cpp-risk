#include <iostream>
#include <Player.hpp>
#include <Player.fwd.hpp>

using namespace std;

/**
 * Creates Player objects.
 * Demonstrates that the player objects indeed have the mentioned features.
 */
int main() {
    Player* p1 = new Player();
    //Player* p2 = new Player();
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Territory* t3 = new Territory("t3");
    Territory* t4 = new Territory("t4");

    //TODO test copy constructor
    // Player* p2 = new Player(*p1);

    

    p1->addTerritory(t1);
    p1->addTerritory(t2);
    p1->addTerritory(t3);
    p1->addTerritory(t4);

    
    p1->printTerritories();

    //TODO toDefend() and toAttack()
    //p1->toDefend();

    /*
    cout << "player 2" << endl;
    p2->printTerritories();
    */

    p1->removeTerritory(t1);
    

    p1->printTerritories();

    /*
    cout << "player 2" << endl;
    p2->printTerritories();
    */

    p1->issueOrder();

    p1->printTerritories();

    delete p1;
    p1 = NULL;
}