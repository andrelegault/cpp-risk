#pragma once

#include "GameObservers.fwd.hpp"
#include <vector>
#include <iostream>
#include "MVC.hpp"
#include "UI.hpp"
#include "GameEngine.hpp"

using namespace UI;
using namespace std;

class PhaseObserver : public Observer, public Component {
public:
    PhaseObserver(GameEngine* gameEngine);

    ~PhaseObserver();

    PhaseObserver(const PhaseObserver& phaseObserver);
    
    friend ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver);

    virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

    virtual Component* clone() const override { return new PhaseObserver(*this); };

protected:
    virtual void update() override {
        this->_update();

        Component::update();
    }

private:
    void _update();

    Component* component = nullptr;
    GameEngine* gameEngine = nullptr;
};

ostream& operator<<(ostream& stream, const PhaseObserver& phaseObserver);

class GameStatisticsObserver : public Observer, public Component {
public:
    GameStatisticsObserver(GameEngine* gameEngine);

    ~GameStatisticsObserver();

    GameStatisticsObserver(const GameStatisticsObserver& gameStatisticsObserver);

    friend ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver);

    virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

    virtual Component* clone() const override { return new GameStatisticsObserver(*this); };

protected:
    virtual void update() override {
        this->_update();
        Component::update();
    }

private:
    void _update();

    Component* component = nullptr;
    GameEngine* gameEngine = nullptr;
};

ostream& operator<<(ostream& stream, const GameStatisticsObserver& gameStatisticsObserver);

class GameUI : public Component {
public:
    GameUI();

    GameUI(vector<Component*> observers);

    ~GameUI();

    GameUI(const GameUI& gameUI);

    friend ostream& operator<<(ostream& stream, const GameUI& gameUI);

    virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

    virtual Component* clone() const override { return new GameUI(*this); };

private:
    Component* component = nullptr;
};

ostream& operator<<(ostream& stream, const GameUI& gameUI);