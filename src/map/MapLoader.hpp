#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include "Utils.hpp"
#include "Map.hpp"

using namespace std;

/**
 * Utility to handle external Domination Map files.
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
     * Loads a Domination Map file into a Map graph.
     *
     * @param file_name Domination Map file to load.
     * @return A Map graph representation of the Conquest Map file.
     * @throw If the file does not exist or the file is invalid.
     */
    static Map* load(const string& fileName);
};

/**
 * Utility to handle external Conquest Map files.
 */
class ConquestFileReader {
private:
    Continent* findContinent(string name);
public:

    /**
     * Default constructor.
     */
    ConquestFileReader();

    /**
     * Copy constructor.
     * @param other Other ConquestFileReader object.
     */
    ConquestFileReader(const ConquestFileReader& other);

    /**
     * Assignment operator overload.
     * @param other Other object used for assignment.
     * @return Reference to shallow copy.
     */
    ConquestFileReader& operator=(const ConquestFileReader& other);

    /**
     * Stream insertion operator.
     * @param stream Stream to output to.
     * @param maploader Object to output.
     */
    friend ostream& operator<<(ostream& stream, const ConquestFileReader& conquestFileReader);

    /**
     * Loads a Conquest Map file into a Map graph.
     *
     * @param file_name Conquest Map file to load.
     * @return A Map graph representation of the Conquest Map file.
     * @throw If the file does not exist or the file is invalid.
     */
    static Map* load(const string& file_name);
};

/**
 * Adapter that extends base MapLoader use the ConquestFileReader
 */
class ConquestFileReaderAdapter: public MapLoader{
private:
    enum MapType {DOMINATION, CONQUEST};
    static MapType checkFileType(const string& file_name);
    static ConquestFileReader conquestFileReader;
public:

    /**
     * Default constructor.
     */
    ConquestFileReaderAdapter();

    /**
     * Copy constructor.
     * @param other Other ConquestFileReader object.
     */
    ConquestFileReaderAdapter(const ConquestFileReaderAdapter& other);

    /**
     * Assignment operator overload.
     * @param other Other object used for assignment.
     * @return Reference to shallow copy.
     */
    ConquestFileReaderAdapter& operator=(const ConquestFileReaderAdapter& other);

    /**
     * Stream insertion operator.
     * @param stream Stream to output to.
     * @param maploader Object to output.
     */
    friend ostream& operator<<(ostream& stream, const ConquestFileReaderAdapter& conquestFileReaderAdapter);

    /**
     * Loads a Map file into a Map graph.
     *
     * @param file_name Conquest/Domination Map file to load.
     * @return A Map graph representation of the Map file.
     * @throw If the file does not exist or the file is invalid.
     */
    static Map* load(const string& file_name);
};
