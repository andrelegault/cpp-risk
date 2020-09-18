#include <iostream>
#include <Map.hpp>

using namespace std;

/**
 * Creates different Map objects.
 * Checks whether they are valid or not.
 */
int main() {
    Map m;

    Continent continent("Contient");

    cout << continent.getName();

    return 0;
}