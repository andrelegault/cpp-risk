#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "Utils.hpp"
#include "Map.hpp"

using namespace std;

/**
 * Utility to handle external Conquest Map files.
 */
class MapLoader {
public:

    /**
     * Default constructor.
     */
    MapLoader();

    /**
     * Copy constructor.
     * @param other Other maploader object.
     */
    MapLoader(const MapLoader& other);

    /**
     * Assignment operator overload.
     * @param other Other object used for assignment.
     * @return Reference to shallow copy.
     */
    MapLoader& operator=(const MapLoader& other);

    /**
     * Stream insertion operator.
     * @param stream Stream to output to.
     * @param maploader Object to output.
     */
    friend ostream& operator<<(ostream& stream, const MapLoader& maploader);

    /**
     * Loads a Conquest Map file into a Map graph.
     *
     * @param file_name Conquest Map file to load.
     * @return A Map graph represenation of the Conquest Map file.
     * @throw If the file does not exist or the file is invalid.
     */
    static Map load(const string fileName);
};
