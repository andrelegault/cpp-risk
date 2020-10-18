#include <GameEngine.hpp>

GameEngine::GameEngine() {
    UI::clear();

    switch (ask(Banner(), { "Start Game", "Exit" })) {
    case 2:
        return;
    }

    string directory = "maps/";

    vector<string> maps;

    for (const auto& entry : directory_iterator(directory)) {
        string path = entry.path();

        auto pathSplit = split(path, "/");

        string file = pathSplit[pathSplit.size() - 1];

        auto fileSplit = split(file, ".");

        string mapName = fileSplit[0];

        string fileExtension = fileSplit[fileSplit.size() - 1];

        if (fileExtension == "map") {
            maps.push_back(mapName);
        }
    }

    UI::clear();

    Map mapObj = MapLoader::load(directory + maps[ask("Select Map", maps) - 1] + ".map");

    if (!mapObj.validate()) {
        UI::clear();

        cout << Grid(new Text("Invalid Map"));

        return;
    }

    this->map = new Map(mapObj);

    this->deck = new Deck();

    UI::clear();

    int numberOfPlayers = range("Number of Players", 2, 5);

    for (int i = 0; i < numberOfPlayers; i++) {
        this->players.push_back(new Player(*this->deck));
    }

    UI::clear();

    bool useObserver = !(ask("Observer?", { "Yes", "No" }) - 1);
}

void GameEngine::startupPhase() {}

void GameEngine::mainGameLoop() {}

void GameEngine::reinforcementPhase() {}

void GameEngine::issueOrdersPhase() {}

void GameEngine::executeOrdersPhase() {}