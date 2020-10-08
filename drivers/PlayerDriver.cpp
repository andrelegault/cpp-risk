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

    Territory* t5 = new Territory("t5");
    Territory* t6 = new Territory("t6");

    //TODO test copy constructor
    Player* p2 = new Player(*p1);

    p1->addTerritory(t1);
    p1->addTerritory(t2);
    p1->addTerritory(t3);
    p1->addTerritory(t4);

    p2->addTerritory(t5);
    p2->addTerritory(t6);

    cout << *p1 << endl;   
    cout << *p2 << endl; 

    //TODO toDefend() and toAttack()
    p1->toDefend();
    p1->toAttack();

    p1->removeTerritory(t1);
    
    cout << *p1 << endl;   
    cout << *p2 << endl;
   
    p1->issueOrder();

    delete p1;
    p1 = NULL;
}