#include "GameObservers.hpp"

/******************************************************
 * PHASE OBSERVER
 *****************************************************/

PhaseObserver::PhaseObserver(GameEngine* gameEngine) : gameEngine(gameEngine), component(nullptr) {};

PhaseObserver::PhaseObserver(const PhaseObserver& phaseObserver) {
    this->gameEngine = phaseObserver.gameEngine;
};

PhaseObserver::~PhaseObserver() {
    delete this->component;
}

void PhaseObserver::update() {
    if (this->gameEngine->currentPlayer == nullptr) return;

    if (this->component != nullptr) delete this->component;

    stringstream ss;

    switch (this->gameEngine->gamePhase) {
    case REINFORCEMENT_PHASE:
        ss << "Available Armies: " << this->gameEngine->currentPlayer->armies << endl << endl;
        ss << Territory::territoryTable(this->gameEngine->currentPlayer->getTerritories(), 4);
        break;
    case ISSUE_ORDER_PHASE:
        for (auto order : this->gameEngine->currentPlayer->orders->getOrders()) {
            ss << *order << endl;
        }
        break;
    case EXECUTE_ORDER_PHASE:
        ss << "Executing orders!" << endl;
        break;
    }

    this->component = new Grid({ {new Text(this->gameEngine->currentPlayer->getName() + ": " + gamePhaseToString(this->gameEngine->gamePhase))}, {new Text(ss.str())} });

    Component::update();

    UI::waitForInput();
}

ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver) {
    if (phaseObserver.component != nullptr) {
        stream << *phaseObserver.component;
    }
    else {
        stream << " ";
    }

    return stream;
}

/******************************************************
 * GAME STATISTICS
 *****************************************************/

GameStatisticsObserver::GameStatisticsObserver(GameEngine* gameEngine) : gameEngine(gameEngine), component(nullptr) {};

GameStatisticsObserver::GameStatisticsObserver(const GameStatisticsObserver& gameStatisticsObserver) {
    this->gameEngine = gameStatisticsObserver.gameEngine;
    this->component = gameStatisticsObserver.component;
};

GameStatisticsObserver::~GameStatisticsObserver() {
    delete this->component;
}

void GameStatisticsObserver::update() {
    if (this->component != nullptr) delete this->component;

    vector<Player*> players = this->gameEngine->players;

    if (players.size() > 1) {
        players.push_back(this->gameEngine->warzoneMap->neutralP);

        vector<vector<Component*>> rows;

        for (auto player : players) {
            rows.push_back(vector<Component*>{ new Text(player->getName()), new Text(to_string(player->getTerritories().size() * 100.0 / this->gameEngine->warzoneMap->getTerritories().size()) + "%") });
        }

        this->component = new Grid({ {new Text("Game Stats")}, {new Grid(rows)} });
    }
    else {
        this->component = new Grid({ {new Text("Game Stats")}, {new Text("Congrats " + players.front()->getName())} });
    }

    Component::update();

    UI::waitForInput();
}

ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver) {
    if (gameStatisticsObserver.component != nullptr) {
        stream << *gameStatisticsObserver.component;
    }
    else {
        stream << " ";
    }

    return stream;
}

/******************************************************
 * GAME UI
 *****************************************************/

GameUI::GameUI() : component(new Text()) {};

GameUI::GameUI(vector<Component*> observers) : component(new Grid({ observers }, UI::Style{ .border = false })) {};

GameUI::GameUI(const GameUI& gameUI) {
    this->component = gameUI.component;
};

GameUI::~GameUI() {
    delete this->component;
}

ostream& operator<<(ostream& stream, const GameUI& gameUI) {
    if (gameUI.component != nullptr) {
        stream << *gameUI.component;
    }

    return stream;
}