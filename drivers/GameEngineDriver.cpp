#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "GameEngine.hpp"
#include "UI.hpp"

using namespace std;
using namespace UI;

int main() {
    GameEngine gameEngine;

    gameEngine.startupPhase();

    return 0;
}
