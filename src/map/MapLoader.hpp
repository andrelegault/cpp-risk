#pragma once

#include <string>

#include <Map.hpp>

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

/**
 * Trim front and end of string of various whitespace characters by regex
 * @param line input
 * @return trimmed string
 */
string trim(string& line);

/**
 * Remove the first word of a string based on a delimiter
 * @param line
 * @param delim
 * @return modified string
 */
string trim_first_word(const string& line, const string& delim = " \t");

/**
 * Split a string into an array of words based on a delimiter
 * @param line
 * @param delim
 * @return vector<string> words
 */
vector<string> split(const string& line, const string& delim = " ");