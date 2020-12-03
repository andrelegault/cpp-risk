#pragma once

#include <vector>
#include <iostream>
#include "GameObservers.fwd.hpp"
#include "MVC.hpp"
#include "UI.hpp"
#include "GameEngine.hpp"

using namespace UI;
using namespace std;

/**
 * Observer displaying information for the current game phase.
 */
class PhaseObserver : public Observer, public Component {
public:
    PhaseObserver(GameEngine* gameEngine);

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver& phaseObserver);

    friend ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver);

    virtual ostream& toString(ostream& stream) const override;

    virtual Component* clone() const override;

protected:
    /// Updates the observer with current phase info.
    virtual void update();

private:
    Component* component = nullptr;
    GameEngine* gameEngine = nullptr;
};

ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver);

/**
 * Observer displaying current game statistics
 */
class GameStatisticsObserver : public Observer, public Component {
public:
    GameStatisticsObserver(GameEngine* gameEngine);

    ~GameStatisticsObserver();

    GameStatisticsObserver(const GameStatisticsObserver& gameStatisticsObserver);

    friend ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver);

    virtual ostream& toString(ostream& stream) const override;

    virtual Component* clone() const override;

protected:
    /// Update the observer with stats for all players.
    virtual void update();

private:
    Component* component;
    GameEngine* gameEngine;
};

ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver);

class GameUI : public Component {
public:
    GameUI();

    GameUI(vector<Component*> observers);

    ~GameUI();

    GameUI(const GameUI& gameUI);

    friend ostream& operator<<(ostream& stream, const GameUI& gameUI);

    virtual ostream& toString(ostream& stream) const override;

    virtual Component* clone() const override;

private:
    Component* component;
};

ostream& operator<<(ostream& stream, const GameUI& gameUI);
