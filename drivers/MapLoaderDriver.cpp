#include <iostream>
#include <MapLoader.hpp>

using namespace std;

/**
 * Reads various files and successfully creates a map object for valid map files.
 * Rejects invalid map files of different kinds. 
 */
int main() {
    // TODO - Read input for file name.
    Map m1 = MapLoader::load("../maps/ameroki.map");

    Map m2 = MapLoader::load("../maps/risk.map");

    Map m3 = MapLoader::load("../maps/world.map");

    // TODO - Better output for maps.
    // cout << &m;

    return 0;
}