#include <iostream>
#include <Player.hpp>
#include <Player.fwd.hpp>

using namespace std;

/**
 * Creates Player objects.
 * Demonstrates that the player objects indeed have the mentioned features.
 */
int main(){
    Player p1;
    cout << "num = " << p1.getNum() << ", name = " << p1.getName() << endl;
}