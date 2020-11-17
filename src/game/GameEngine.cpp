#include "GameEngine.hpp"

string gamePhaseToString(GamePhase gamePhase) {
    switch (gamePhase) {
    case STARTUP_PHASE: return "Startup Phase";
    case REINFORCEMENT_PHASE: return "Reinforcement Phase";
    case ISSUE_ORDER_PHASE: return "Issue Order Phase";
    case EXECUTE_ORDER_PHASE: return "Execute Order Phase";
    default: return "Unknown Phase";
    }
}

GameEngine::GameEngine() : deck(new Deck()), warzoneMap(nullptr), gameUI(nullptr), gamePhase(STARTUP_PHASE), currentPlayer(nullptr) {}

GameEngine::~GameEngine() {
    delete this->deck;

    for (auto player : this->players) delete player;

    delete this->warzoneMap;

    delete this->gameUI;
}

int GameEngine::getPlayerArmyCount(int numberOfPlayers) const {
    return std::max(50 - (numberOfPlayers * 5), 25);
}

void GameEngine::initPlayers() {
    std::shuffle(this->players.begin(), this->players.end(), std::random_device{});

    int armyCount = this->getPlayerArmyCount(this->players.size());

    for (auto player : this->players) {
        player->armies += armyCount;
    }
}

void GameEngine::setGamePhase(GamePhase gamePhase) {
    this->gamePhase = gamePhase;
}

void GameEngine::setCurrentPlayer(Player* player) {
    this->currentPlayer = player;
}

Player* GameEngine::getCurrentPlayer() {
    return this->currentPlayer;
}

GamePhase GameEngine::getGamePhase() {
    return this->gamePhase;
}

void GameEngine::assignTerritories() {
    vector<Territory*> territories = this->warzoneMap->getTerritories();

    std::shuffle(territories.begin(), territories.end(), std::random_device{});

    int roundRobin = 0;

    int numberOfPlayers = this->players.size();

    for (auto territory : territories) {
        this->players.at(roundRobin++ % numberOfPlayers)->addTerritory(territory);
    }
}

void GameEngine::printTerritories() {
    vector<vector<UI::Component*>> table;

    for (auto continent : this->warzoneMap->getContinents()) {
        table.push_back({ new UI::Text(continent->getName()) });

        vector<string> territoryList;
        for (auto territory : continent->getTerritories()) {
            stringstream ss;

            ss << territory->getOwnerName() << " -> " << territory->getName() << " -> " << territory->getNumberOfArmies();

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

    this->warzoneMap = new Map(mapObj);

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
            this->attach(phaseObserver);
        }
    }

    if (!(ask("Game Statistics Observer?", { "Yes", "No" }) - 1)) {
        GameStatisticsObserver* gameStatisticsObserver = new GameStatisticsObserver(this);
        observers.push_back(gameStatisticsObserver);

        for (auto player : this->players) {
            player->attach(gameStatisticsObserver);
            this->attach(gameStatisticsObserver);
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
    this->initPlayers();
    this->assignTerritories();

    //TODO: make sure that "all players have all the orders for playing in a turn"

    this->mainGameLoop();
}

void GameEngine::mainGameLoop() {
    while (true) {
        for (auto& entry : GameEngine::immunities) {
            entry.second = true;
        }

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
            cout << this->players.front()->getName() << " wins!" << endl;
            this->notify();

            return;
        }
        else if (this->players.size() == 0) {
            return;
        }

        // Phases.
        this->reinforcementPhase();
        this->issueOrdersPhase();
        this->executeOrdersPhase();

        for (auto player : this->players) {
            cout << player->getName() << " has " << player->getTerritories().size() << " territories" << endl;
        }

        vector<tuple<Player*, Player*>> toErase;

        for (auto entry : GameEngine::immunities) {
            if (entry.second) toErase.push_back(entry.first);
        }
        
        for (auto tup : toErase) {
            GameEngine::immunities.erase(tup);
        }
    }
}

void GameEngine::reinforcementPhase() {
    this->setGamePhase(REINFORCEMENT_PHASE);

    for (auto player : this->players) {
        this->setCurrentPlayer(player);

        player->armies += std::max((player->getNumTerritories() / 3), 3);

        for (auto continent : this->warzoneMap->getContinents()) {
            bool hasAllTerritories = true;

            for (auto territory : continent->getTerritories()) {
                if (territory->getOwner() == player) {
                    hasAllTerritories = false;
                    break;
                }
            }

            if (hasAllTerritories) {
                player->armies += continent->getBonus();
            }
        }

        this->notify();
    }
}

void GameEngine::issueOrdersPhase() {
    this->setGamePhase(ISSUE_ORDER_PHASE);

    for (auto player : this->players) {
        this->setCurrentPlayer(player);

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
    this->setGamePhase(EXECUTE_ORDER_PHASE);

    int playersDoneDeploying = 0;
    int numPlayers = this->players.size();

    while (playersDoneDeploying < numPlayers) {
        for (auto player : this->players) {
            this->setCurrentPlayer(player);

            Order* nextDeployed = player->getNextOrder(1);

            if (nextDeployed == nullptr) {
                playersDoneDeploying++;
            }
            else {
                // cout << *nextDeployed << endl;
                nextDeployed->execute();

                player->removeOrder(nextDeployed);
            }
        }
    }

    while (!this->isExecutionDone()) {
        for (auto player : this->players) {
            this->setCurrentPlayer(player);

            Order* nextOrder = player->getNextOrder();

            if (nextOrder != nullptr) {
                // cout << *nextOrder << endl;
                nextOrder->execute();

                player->removeOrder(nextOrder);
            }
        }
    }
}

map<tuple<Player*, Player*>, bool> GameEngine::immunities;
