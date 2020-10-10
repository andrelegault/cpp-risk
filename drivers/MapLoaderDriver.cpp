#include <iostream>
#include <MapLoader.hpp>

using namespace std;


void basicTest() {
    Map m1 = MapLoader::load("maps/ameroki.map");

    Map m2 = MapLoader::load("maps/risk.map");

    Map m3 = MapLoader::load("maps/world.map");

    cout << m1 << endl << endl;
    cout << m2 << endl << endl;
    cout << m3 << endl << endl;
}

void inputTest() {
    string path;
    cout << "\n\nInput the path to a map file you want to load: \n";
    cin >> path;
    cout << "\nLoading " + path + "...\n";

    try {
        Map m = MapLoader::load(path);
        cout << m << endl << endl;
    }
    catch (logic_error& error) {
        cerr << "exception: " << error.what() << endl;
    }
}

/**
 * Reads various files and successfully creates a map object for valid map files.
 * Rejects invalid map files of different kinds.
 */
int main() {
    bool exit = false;
    int input;
    while (!exit) {
        // present options
        cout << "Choose an option:\n";
        cout << "1 - test 3 maps\n";
        cout << "2 - input path to map\n";
        cout << "3 - exit\n";

        // collect input
        cin >> input;

        // do things
        switch (input) {
        case 1: basicTest(); break;
        case 2: inputTest(); break;
        default: cout << "Invalid Input!!\n\n";
        case 3: exit = true;
        }
    }

    return 0;
}
