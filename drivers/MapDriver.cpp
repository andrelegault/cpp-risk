#include <iostream>
#include <string>
#include <Map.hpp>
#include <MapLoader.hpp>

using namespace std;

/**
 * Creates different Map objects.
 * Checks whether they are valid or not.
 */
int main() {
    // TODO - Read input for file name.
    Map m = MapLoader::load("file");

    // TODO - Better output for maps.
    // cout << &m;

    return 0;
}