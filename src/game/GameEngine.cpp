#include "GameEngine.hpp"

GameEngine::GameEngine() : deck(new Deck()), map(nullptr), gameUI(nullptr) {}

GameEngine::~GameEngine() {
    delete this->deck;

    delete this->map;

    for (auto player : this->players) {
        delete player;
    }

    delete this->gameUI;
}

int GameEngine::getPlayerArmyCount(int numberOfPlayers) const {
    return std::max(50 - (numberOfPlayers * 5), 25);
}

void GameEngine::initPlayers() {
    std::shuffle(this->players.begin(), this->players.end(), std::random_device{});

    int armyCount = this->getPlayerArmyCount(this->players.size());

    for (auto player : this->players) {
        player->addArmies(armyCount);
    }
}

void GameEngine::assignTerritories() {
    vector<Territory*> territories = this->map->getTerritories();

    std::shuffle(territories.begin(), territories.end(), std::random_device{});

    int roundRobin = 0;

    int numberOfPlayers = this->players.size();

    for (auto territory : territories) {
        this->players[roundRobin++ % numberOfPlayers]->addTerritory(territory);
    }
}

void GameEngine::printTerritoryOwners() {
    for (auto continent : this->map->getContinents()) {
        cout << continent->getName() << ":" << endl;

        for (auto territory : continent->getTerritories()) {
            cout << "\t" << territory->getName() << ", owned by " << territory->getOwnerName() << endl;
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

void GameEngine::init() {
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
        Player* player = new Player(*this->deck);

        this->players.push_back(player);
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

    this->startupPhase();
}

void GameEngine::startupPhase() {
    initPlayers();

    assignTerritories();

    printTerritoryOwners();

    //TODO make sure that "all players have all the orders for playing in a turn"

    this->mainGameLoop();
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
        player->addArmies(std::max((int)floor(player->getNumTerritories() / 3), 3));

        for (auto continent : this->map->getContinents()) {
            bool hasAllTerritoires = true;

            for (auto territory : continent->getTerritories()) {
                if (territory->getOwner() == player) {
                    hasAllTerritoires = false;
                    break;
                }
            }

            if (hasAllTerritoires) {
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
        Order* nextOrder = player->getNextOrder();
        nextOrder->execute();
        // Execute all deploy orders.
    }

    for (auto player : this->players) {
        // Execute orders according to priority.
    }
}