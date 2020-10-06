#include <iostream>
#include <string>
#include <Map.hpp>

using namespace std;

/**
 * Creates different Map objects.
 * Checks whether they are valid or not.
 */
int main() {
    // Make map.
    Map m("Map");

    // Make continent
    Continent* c1 = new Continent("Continent 1");
    Continent* c2 = new Continent("Continent 2");

    // Connecting continents together (only one needs to be done).
    ((MapNode*) c1)->connect(c2);
    // This should not do anything (because MapNode already connected).
    ((MapNode*) c2)->connect(c1);

    // Make territory
    Territory* t1 = new Territory("Territory 1");
    Territory* t2 = new Territory("Territory 2");

    // Connecting territories together (only one needs to be done).
    ((MapNode*) t1)->connect(t2);

    // Connecting territory to continent
    c1->connect(t1);
    c2->connect(t2);

    // Connecting continent to map (needs to be done for all continents).
    m.connect(c1);
    m.connect(c2);

    // Print continents
    for(auto continent : m.getContinents()) {
        cout << continent;
    }

    // Print territories
    for(auto territory : m.getTerritories()) {
        cout << territory;
    }

    // Or you can use the print map
    // cout << &m;

    // Validating map
    cout << "Map Valid: " << m.validate() << endl;

    return 0;
}