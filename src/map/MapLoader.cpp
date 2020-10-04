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

    return m;
}