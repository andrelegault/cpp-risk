#include "GameEngine.hpp"

GameEngine::GameEngine() {
    switch (ask(Banner(), { "Start Game", "Exit" })) {
    case 2:
        return;
    }

    string directory = "maps/";

    vector<string> maps;

    for (const auto& entry : directory_iterator(directory)) {
        string path = entry.path();

        auto pathSplit = Utils::split(path, "/");

        string file = pathSplit[pathSplit.size() - 1];

        auto fileSplit = Utils::split(file, ".");

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

    int numberOfPlayers = range("Number of Players", 2, 5);

    for (int i = 0; i < numberOfPlayers; i++) {
        this->players.push_back(new Player(*this->deck));
    }

    vector<Component*> observers;

    if(!(ask("Phase Observer?", { "Yes", "No" }) - 1)) {
        PhaseObserver* phaseObserver = new PhaseObserver(this);
        observers.push_back(phaseObserver);

        for(auto player : this->players) {
            player->attach(phaseObserver);
        }
    }

    if(!(ask("Game Statistics Observer?", { "Yes", "No" }) - 1)) {
        GameStatisticsObserver* gameStatisticsObserver = new GameStatisticsObserver(this);
        observers.push_back(gameStatisticsObserver);

        for(auto player : this->players) {
            player->attach(gameStatisticsObserver);
        }
    }

    if(!observers.empty()) {
        this->gameUI = new GameUI(observers);
    } else {
        this->gameUI = new GameUI();
    }

    this->gameUI->update();
}

GameEngine::~GameEngine() {}

void GameEngine::startupPhase() {

    //shuffle the players in the list to get random order
    std::random_shuffle(players.begin(), players.end());

    //set number of armies based on number of players
    switch(numberOfPlayers){
        case 2:
            for(auto player : this->players) {
                player->addArmies(40);
            }
            break;
        case 3:
            for(auto player : this->players) {
                player->addArmies(35);
            }
            break;
        case 4:
            for(auto player : this->players) {
                player->addArmies(30);
            }
            break;
        case 5:
            for(auto player : this->players) {
                player->addArmies(25);
            }
            break;
    }

    //TODO distribute territories between players

    //TODO make sure that "all players have all the orders for playing in a turn"
}

void GameEngine::mainGameLoop() {}

void GameEngine::reinforcementPhase() {

    //TODO add armies based on continent control bonus
    for(auto player : this->players) {
        //make sure player gets at least 3 new armies
        if (player.getNumTerritories()/3 < 3){
            player->addArmies(3);
        }
        else{
            player->addArmies(player.getNumTerritories()/3);
        }
    }

}

void GameEngine::issueOrdersPhase() {}

void GameEngine::executeOrdersPhase() {}