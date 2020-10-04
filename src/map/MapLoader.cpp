#include <MapLoader.hpp>

// TODO: Loading map.
Map MapLoader::load(string file) {
    string mapName = "Map Name";

    // Parse File
    //      Get Map Name
    //      Map Territory name to Territory instance.
    //      Map Continent name to Continent instance.
    Map m(mapName);

    // Connect Territories instances using mapped names.

    // Connect Continents instance using mapped names.

    // Connect Territory instances to Continent instances.

    // Connect Continent instances to Map instance.

    // Basic example:

    // Make continent
    Continent* c1 = new Continent("Continent 1");
    Continent* c2 = new Continent("Continent 2");

    // Connecting continents together (only one needs to be done).
    ((MapNode*) c1)->connect(c2);

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

    // Validating map
    cout << "Map Valid: " << m.validate() << endl << endl;

    // FOR DEBUG -> use driver.
    for(auto continent : m.getContinents()) {
        cout << continent;
    }

    // FOR DEBUG -> use driver.
    for(auto territory : m.getTerritories()) {
        cout << territory;
    }

    return m;
}