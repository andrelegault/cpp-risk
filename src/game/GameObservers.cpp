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
    if (this->component != nullptr) delete this->component;

    if(this->gameEngine->currentPlayer != nullptr) {
        stringstream ss;

        ss << this->gameEngine->currentPlayer->getName() << ": " << gamePhaseToString(this->gameEngine->gamePhase);

        this->component = new Text(ss.str());
    }

    Component::update();

    UI::waitForInput();
}

ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver) {
    if (phaseObserver.component != nullptr) {
        stream << *phaseObserver.component;
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

    vector<vector<Component*>> rows;

    for (auto player : this->gameEngine->players) {
        rows.push_back(vector<Component*>{ new Text(player->getName()), new Text(to_string(player->getTerritories().size() * 100.0 / this->gameEngine->map->getTerritories().size()) + "%") });
    }

    this->component = new Grid(rows);

    Component::update();

    UI::waitForInput();
}

ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver) {
    if (gameStatisticsObserver.component != nullptr) {
        stream << *gameStatisticsObserver.component;
    }

    return stream;
}

/******************************************************
 * GAME UI
 *****************************************************/

GameUI::GameUI() : component(new Text()) {};

GameUI::GameUI(vector<Component*> observers) : component(new Grid({ observers })) {};

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