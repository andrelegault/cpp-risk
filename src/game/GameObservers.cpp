#include "GameObservers.hpp"

PhaseObserver::PhaseObserver(GameEngine* gameEngine) : gameEngine(gameEngine) {
    this->_update();
};

// TODO
PhaseObserver::PhaseObserver(const PhaseObserver& phaseObserver) {};

PhaseObserver::~PhaseObserver() {
    delete this->component;
}

void PhaseObserver::_update() {
    if (this->component != nullptr) {
        delete this->component;
    }

    this->component = new Text("HI");
}

ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver) {
    if (phaseObserver.component != nullptr) {
        stream << *phaseObserver.component;
    }

    return stream;
}

GameStatisticsObserver::GameStatisticsObserver(GameEngine* gameEngine) : gameEngine(gameEngine) {
    this->_update();
};

// TODO
GameStatisticsObserver::GameStatisticsObserver(const GameStatisticsObserver& gameStatisticsObserver) {};

GameStatisticsObserver::~GameStatisticsObserver() {
    delete this->component;
}

void GameStatisticsObserver::update() {
    this->_update();
    Component::update();
}

void GameStatisticsObserver::_update() {
    if (this->component != nullptr) {
        delete this->component;
    }

    vector<vector<Component*>> rows;

    for (auto player : this->gameEngine->players) {
        rows.push_back(vector<Component*>{ new Text(player->getName()), new Text(to_string(player->getTerritories().size())) });
    }

    this->component = new Grid(rows);
}

ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver) {
    if (gameStatisticsObserver.component != nullptr) {
        stream << *gameStatisticsObserver.component;
    }

    return stream;
}

GameUI::GameUI() : component(new Text()) {};

GameUI::GameUI(vector<Component*> observers) : component(new Grid({ observers })) {};

// TODO
GameUI::GameUI(const GameUI& gameUI) {};

GameUI::~GameUI() {
    delete this->component;
}

ostream& operator<<(ostream& stream, const GameUI& gameUI) {
    if (gameUI.component != nullptr) {
        stream << *gameUI.component;
    }

    return stream;
}