#include <iostream>
#include <string>
#include <Map.hpp>
#include <assert.h>

using namespace std;

/**
 * Creates different Map objects.
 * Checks whether they are valid or not.
 */
int main() {
    cout << "++++++++++++++" << endl;
    cout << "+ Simple Map +" << endl;
    cout << "++++++++++++++" << endl << endl;

    // Make map.
    Map m("Map");

    // Make continent
    Continent* c1 = new Continent("Continent 1");
    Continent* c2 = new Continent("Continent 2");

    // Connecting continents together (only one needs to be done).
    ((MapNode*)c1)->connect(c2);
    // This should not do anything (because MapNode already connected).
    ((MapNode*)c2)->connect(c1);

    // Make territory
    Territory* t1 = new Territory("Territory 1");
    Territory* t2 = new Territory("Territory 2");

    // Connecting territories together (only one needs to be done).
    ((MapNode*)t1)->connect(t2);

    // Connecting territory to continent
    c1->connect(t1);
    c2->connect(t2);

    // Connecting continent to map (needs to be done for all continents).
    m.connect(c1);
    m.connect(c2);

    // Prints simple map.
    cout << m;

    // Validating map
    assert(m.validate());

    cout << "Map Valid: " << m.validate() << endl << endl;

    cout << "+++++++++++++++++" << endl;
    cout << "+ Map Deep Copy +" << endl;
    cout << "+++++++++++++++++" << endl << endl;

    // Deep copy of Map.
    Map m2 = Map(m);

    // Removing element from copy (if shallow copy, should show up in map).
    m2.remove(m2.getContinents()[0]);

    assert(m.getContinents().size() == 2);
    assert(m2.getContinents().size() == 1);

    // Printing maps with address.
    cout << &m << " " << m;
    cout << &m2 << " " << m2;


    cout << "+++++++++++++++++++++++++" << endl;
    cout << "+ Map Element Deep Copy +" << endl;
    cout << "+++++++++++++++++++++++++" << endl << endl;

    // Taking copy of first contient.
    Continent* cc = new Continent(*c1);
    // Removing first territory (if shallow copy, should show up in map).
    cc->remove(cc->getTerritories()[0]);

    assert(m.getContinents()[0]->getTerritories().size() == 1);

    cout << m;

    delete cc;

    // Removing map.
    delete t1;
    delete t2;
    delete c1;
    delete c2;

    return 0;
}
