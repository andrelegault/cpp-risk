#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <GameEngine.hpp>
#include <UI.hpp>

using namespace std;
using namespace UI;

int main() {
    UI::clear();

    // GameEngine gameEngine;
    // stringstream ss;

    // ss << UI::Banner();

    // switch(UI::ask(ss.str(), {"Start Game", "Exit"})) {
    //    case 1:
    //        UI::clear();
    //        UI::range("How many players?", 2, 5);
    //        break;
    // }

    Viewport* viewport = new Viewport(
        new Grid({ {
            new Text("Hello World"),
            new List({"Hello Friend"})
        } })
        , { .width = 10, .height = 5 });

    Frame frame(viewport);

    for (int i = 0; i < 100; i++) {
        viewport->style.x = i;

        UI::clear();
        cout << frame;

        usleep(100000);
    }

    delete viewport;

    return 0;
}