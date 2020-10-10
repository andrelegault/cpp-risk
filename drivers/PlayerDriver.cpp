#include <iostream>
#include <Player.hpp>
#include <Player.fwd.hpp>

using namespace std;

/**
 * Creates Player objects.
 * Demonstrates that the player objects indeed have the mentioned features.
 */
int main() {
    Player* p0 = new Player();
    Territory* t1 = new Territory("t1");
    Territory* t2 = new Territory("t2");
    Territory* t3 = new Territory("t3");
    Territory* t4 = new Territory("t4");

    Territory* t5 = new Territory("t5");
    Territory* t6 = new Territory("t6");

    // TODO test copy constructor
    Player* p1 = new Player(*p0);

    p0->addTerritory(t1);
    p0->addTerritory(t2);
    p0->addTerritory(t3);
    p0->addTerritory(t4);

    p1->addTerritory(t5);
    p1->addTerritory(t6);

    cout << *p0 << endl;
    cout << *p1 << endl;

    p0->toDefend();
    p0->toAttack();

    p0->removeTerritory(t1);

    cout << *p0 << endl;
    cout << *p1 << endl;

    p0->issueOrder();

    delete p0;

    delete p1;
}