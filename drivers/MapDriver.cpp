#include <iostream>
#include <WarZone/Map.h>
#include <WarZone/Territory.h>
#include <WarZone/Continent.h>

using namespace std;

/**
 * Creates different Map objects.
 * Checks whether they are valid or not.
 */
int main() {
    Map map = Map();
    Continent continent = Continent("Continent");
    Territory territory = Territory("Territory");
    
    continent.add(&territory);
    map.add(&continent);

    cout << &map;

    return 0;
}