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

map<tuple<Player*, Player*>, bool> GameEngine::immunities;

GameEngine::GameEngine() : deck(new Deck()), warzoneMap(nullptr), gameUI(nullptr), gamePhase(STARTUP_PHASE), currentPlayer(nullptr) {
}

ostream& operator<<(ostream& stream, const GameEngine& gameEngine) {
    stream << "Game engine" << endl;
    return stream;
}

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
        player->armies = armyCount;
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

    // every territory is assigned to a player (with a 20% chance of every territory being assigned to the neutral player)
    for (auto territory : territories) {
        // give the neutral player some territories
        if (Utils::getRandom(1, 10) < 3)
            Map::neutralP->addTerritory(territory);
        else
            this->players.at(roundRobin++ % numberOfPlayers)->addTerritory(territory);
    }
}

void GameEngine::init() {
    switch (ask(Banner(), { "Start Game", "Exit" })) {
    case 2:
        return;
    default:
        break;
    }

    string directory;
    switch (ask(Banner(), { "Domination Maps", "Conquest Maps", "All Maps" })) {
    case 1:
        directory = "maps/domination/";
        break;
    case 2:
        directory = "maps/conquest/";
        break;
    default:
        directory = "maps/";
        break;
    }

    vector<string> maps;
    vector<string> mapNames;

    for (const auto& entry : recursive_directory_iterator(directory)) {
        const path& mapPath = entry.path();

        if (mapPath.extension() == ".map") {
            mapNames.emplace_back(mapPath.stem());
            maps.emplace_back(mapPath.string());
        }
    }

    UI::clear();

    if (directory == "maps/dominion/") {
        this->warzoneMap = MapLoader::load(maps.at(ask("Select Map", mapNames) - 1));
    }
    else { // The Adapter is capable of falling back to the parent MapLoader if it detects a Dominion map
        this->warzoneMap = ConquestFileReaderAdapter::load(maps.at(ask("Select Map", mapNames) - 1));
    }

    if (!this->warzoneMap->validate()) {
        UI::clear();
        cout << *this->warzoneMap << endl;
        cout << Grid(new Text("Invalid Map"));

        throw logic_error("Map Validation Failed");
    }

    UI::clear();

    int numberOfPlayers = range("Number of Players", 2, 5);

    for (int i = 0; i < numberOfPlayers; ++i) {
        bool isAggressive = rand() % 2 == 0;
        PlayerStrategy* strat = nullptr;
        if (isAggressive) strat = new AggressivePlayerStrategy();
        else strat = new BenevolentPlayerStrategy();
        Player* player = new Player(this->deck, strat);
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
    this->mainGameLoop();
}

void GameEngine::mainGameLoop() {
    const int totalTerritories = this->warzoneMap->getTerritories().size();
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

        int changeStrategies = Utils::getRandom(1, 100);
        // 1% chance that player strategies change
        if (changeStrategies == 1) {
            cout << "CHANGING PLAYER STRATEGIES DYNAMICALLY" << endl;
            // exclude neutral player strategy as we already have a neutral player
            int randomIndex = Utils::getRandom(0, PlayerStrategy::numStrategies - 2);
            for(auto player: this->players)
                player->setStrategy(PlayerStrategy::fromInt(randomIndex));
        }

        // Checks win condition.

        // remaining player has to have conquered the whole map to win
        if (this->players.size() == 1 && this->players.front()->getTerritories().size() == totalTerritories) {
            cout << this->players.front()->getName() << " wins!" << endl;
            this->notify();

            return;
        }
        else if (this->players.size() == 0) {
            cout << "no players left" << endl;
            return;
        }

        // Phases.
        this->reinforcementPhase();
        this->issueOrdersPhase();
        this->executeOrdersPhase();

        for (auto player : this->players) {
            cout << player->getName() << "(" << player->getStrategy() << ")" << " has " << player->getTerritories().size() << " territories" << endl;
        }

        cout << Map::neutralP->getName() << " has " << Map::neutralP->getTerritories().size() << " territories" << endl;

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
        if (player->remainingOrders() > 0)
            return false;
    }

    return true;
}

void GameEngine::executeOrdersPhase() {
    this->setGamePhase(EXECUTE_ORDER_PHASE);

    // Deploys don't affect other players so we can just deploy them all for each player one shot.
    for (auto player : this->players) {
        this->setCurrentPlayer(player);

        Order* nextDeployed = player->getNextOrder(1);

        while (nextDeployed != nullptr) {
            nextDeployed->execute();

            player->removeOrder(nextDeployed);
            delete nextDeployed;

            nextDeployed = player->getNextOrder(1);
        }

        this->notify();
    }

    while (!this->isExecutionDone()) {
        for (auto player : this->players) {
            this->setCurrentPlayer(player);

            Order* nextOrder = player->getNextOrder();

            if (nextOrder != nullptr) {
                nextOrder->execute();

                player->removeOrder(nextOrder);
                delete nextOrder;
            }
        }
    }
}