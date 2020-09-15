#include <string>
#include <Map.h>

using namespace std;

/**
 * Utility to handle external Conquest Map files.
 */
class MapLoader {
    public:
        /**
         * Loads a Conquest Map file into a Map graph.
         * 
         * @param file_name Conquest Map file to load.
         * @return A Map graph represenation of the Conquest Map file.
         * @throw If the file does not exist or the file is invalid.
         */
        static Map load(string file_name);
};