#include "GameEngine.hpp"

GameEngine::GameEngine() : deck(new Deck()), map(nullptr), gameUI(nullptr) {}

GameEngine::~GameEngine() {
    delete this->deck;

    delete this->map;

    for (auto player : this->players) delete player;

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
        this->players.at(roundRobin++ % numberOfPlayers)->addTerritory(territory);
    }
}

void GameEngine::printTerritoryOwners() {
    vector<vector<UI::Component*>> table;

    for (auto continent : this->map->getContinents()) {
        table.push_back({ new UI::Text(continent->getName()) });

        vector<string> territoryList;
        for (auto territory : continent->getTerritories()) {
            stringstream ss;

            ss << territory->getOwnerName() << " -> " << territory->getName();

            territoryList.push_back(ss.str());
        }

        table.push_back({ new UI::List(territoryList) });
    }

    cout << Grid(table);
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

    Map mapObj = MapLoader::load(directory + maps.at(ask("Select Map", maps) - 1) + ".map");

    if (!mapObj.validate()) {
        UI::clear();

        cout << Grid(new Text("Invalid Map"));

        return;
    }

    this->map = new Map(mapObj);

    int numberOfPlayers = range("Number of Players", 2, 5);

    for (int i = 0; i < numberOfPlayers; ++i) {
        Player* player = new Player(this->deck);

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

    UI::clear();

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
    while (true) {
        // Removes losing players.
        std::vector<Player*> playersWithTerritory;

        for (auto player : this->players) {
            if (player->getTerritories().size() > 0) {
                playersWithTerritory.push_back(player);
            }
            else {
                delete player;
            }
        }

        this->players = playersWithTerritory;

        // Checks win condition.
        if (this->players.size() == 1) {
            cout << "WINNER " << this->players.front() << endl;
            return;
        }
        else if (this->players.size() == 0) {
            return;
        }

        // Phases.
        this->reinforcementPhase();
        this->issueOrdersPhase();
        this->executeOrdersPhase();
    }
}

void GameEngine::reinforcementPhase() {
    for (auto player : this->players) {
        player->addArmies(std::max((int)floor(player->getNumTerritories() / 3), 3));

        for (auto continent : this->map->getContinents()) {
            bool hasAllTerritories = true;

            for (auto territory : continent->getTerritories()) {
                if (territory->getOwner() == player) {
                    hasAllTerritories = false;
                    break;
                }
            }

            if (hasAllTerritories) {
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

bool GameEngine::isExecutionDone() const {
    for (auto player : this->players) {
        if (player->remainingOrders() > 0) {
            return false;
        }
    }

    return true;
}

void GameEngine::executeOrdersPhase() {
    int playersDoneDeploying = 0;
    int numPlayers = this->players.size();

    while (playersDoneDeploying < numPlayers) {
        for (auto player : this->players) {
            Order* nextDeployed = player->getNextOrder(1);

            if (nextDeployed == nullptr) {
                playersDoneDeploying++;
            }
            else {
                cout << "NEXT DEPLOYED" << endl;
                cout << *nextDeployed << endl;
                nextDeployed->execute();
                player->removeOrder(nextDeployed);
            }
        }
    }

    while (!this->isExecutionDone()) {
        for (auto player : this->players) {
            Order* nextOrder = player->getNextOrder();

            if (nextOrder != nullptr) {
                cout << "NEXT DEPLOYED" << endl;
                cout << *nextOrder << endl;
                nextOrder->execute();
                player->removeOrder(nextOrder);
            }
        }
    }
}