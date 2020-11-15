#include "GameEngine.hpp"

GameEngine::GameEngine() : deck(new Deck()) {
    switch (ask(Banner(), { "Start Game", "Exit" })) {
    case 2:
        return;
    default:
        break;
    }

    string directory = "maps/";

    vector<string> maps;

    for (const auto& entry : directory_iterator(directory)) {
        const path mapPath = entry.path();

        if (mapPath.extension() == ".map") {
            cout << mapPath.stem() << endl;
            maps.push_back(mapPath.stem());
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

    int numberOfPlayers = range("Number of Players", 2, 5);

    for (int i = 0; i < numberOfPlayers; ++i) {
        this->players.push_back(new Player(*this->deck));
    }

    vector<Component*> observers;

    if (!(ask("Phase Observer?", { "Yes", "No" }) - 1)) {
        PhaseObserver* phaseObserver = new PhaseObserver(this);
        observers.push_back(phaseObserver);

        for (auto player : this->players) {
            player->attach(phaseObserver);
        }
    }

    if (!(ask("Game Statistics Observer?", { "Yes", "No" }) - 1)) {
        GameStatisticsObserver* gameStatisticsObserver = new GameStatisticsObserver(this);
        observers.push_back(gameStatisticsObserver);

        for (auto player : this->players) {
            player->attach(gameStatisticsObserver);
        }
    }

    if (!observers.empty()) {
        this->gameUI = new GameUI(observers);
    }
    else {
        this->gameUI = new GameUI();
    }

    this->gameUI->update();
}

GameEngine::~GameEngine() {
    delete this->deck;
    delete this->map;

    for (auto player : this->players) {
        delete player;
    }

    delete this->gameUI;
}

int GameEngine::getPlayerArmyCount(int numberOfPlayers) const {
    // TODO: throw error if numberOfPlayers isn't within acceptable range
    return 2 <= numberOfPlayers <= 5 ? 50 - (numberOfPlayers * 5) : -1;
}

void GameEngine::startupPhase() {
    //shuffle the players in the list to get random order
    std::random_device rd;
    std::shuffle(this->players.begin(), this->players.end(), rd);

    int armyCount = this->getPlayerArmyCount(this->players.size());

    //set number of armies based on number of players
    for (auto player : this->players) {
        player->addArmies(armyCount);
    }

    assignTerritories();

    printTerritoryOwners();

    //TODO make sure that "all players have all the orders for playing in a turn"
}

void GameEngine::assignTerritories() {
    srand(time(NULL));
    vector<Territory*> territories = this->map->getTerritories();
    const int numTerritories = territories.size();
    const int numPlayers = this->players.size();
    // vector containing {0, 1, ..., numTerritories - 1}
    vector<int> avail(numTerritories);
    for (int i = 0; i < numTerritories; ++i) {
        avail[i] = i;
    }

    int roundRobin = 0;
    while (avail.size() > 0) {
        // get a random index from any that are left
        const int rIndex = rand() % avail.size();
        const int tIndex = avail.at(rIndex);
        // get the pointer using random value
        Territory* rTerritory = territories.at(tIndex);
        // add it to the player's territories
        this->players.at(++roundRobin % numPlayers)->addTerritory(rTerritory);
        // remove it from the list of available indexes
        avail.erase(avail.begin() + rIndex);
    }
}

void GameEngine::printTerritoryOwners(){
     for (Continent* continent : this->map->getContinents()){
            cout << continent->getName() << ":" << endl;
            for (Territory* territory : continent->getTerritories()){
                cout << "\t" << territory->getName() << " , owned by " << territory->getOwnerName() << endl;
            }
        }
}

Player* GameEngine::getWinningPlayer() {
    Player* lastPlayerWithTerritories = nullptr;
    int numberOfPlayersWithTerritories;

    for (auto player : this->players) {
        if (player->getNumTerritories() > 0) {
            lastPlayerWithTerritories = player;
            numberOfPlayersWithTerritories++;
        }
    }

    if (numberOfPlayersWithTerritories == 1) {
        return lastPlayerWithTerritories;
    }
    else {
        return nullptr;
    }
}

void GameEngine::mainGameLoop() {
    this->reinforcementPhase();
    this->issueOrdersPhase();
    this->executeOrdersPhase();

    Player* winningPlayer = this->getWinningPlayer();

    if (winningPlayer != nullptr) {
        cout << "WINNER " << winningPlayer << endl;
        return;
    }

    this->mainGameLoop();
}

void GameEngine::reinforcementPhase() {
    for (auto player : this->players) {
        // add armies based on current number of owned territories
        player->addArmies(std::max((int)floor(player->getNumTerritories() / 3), 3));

        for (auto continent : this->map->getContinents()) {
            bool hasAllTerritoires = true;

            for (auto territory : continent->getTerritories()) {

                // why not just check if the territory's owner == player?, i.e., territory->playerOwner == player
                if (territory->getOwner() == player) {
                    hasAllTerritoires = false;
                    break;
                }
            }

            if (hasAllTerritoires) {
                // add bonus if applicable
                player->addArmies(continent->getBonus());
            }
        }
    }
}

void GameEngine::issueOrdersPhase() {
    for (auto player : this->players) {
        player->issueOrder();
    }
}

void GameEngine::executeOrdersPhase() {
    for (auto player : this->players) {
        // Execute all deploy orders.
    }

    for (auto player : this->players) {
        // Execute orders according to priority.
    }
}