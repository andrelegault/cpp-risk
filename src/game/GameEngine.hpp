#pragma once

#include <filesystem>
#include <math.h>
#include <algorithm>
#include <random>
#include "GameEngine.fwd.hpp"
#include "UI.hpp"
#include "Player.hpp"
#include "Utils.hpp"
#include "MapLoader.hpp"
#include "Map.hpp"
#include "Cards.hpp"
#include "GameObservers.hpp"

using namespace filesystem;
using namespace UI;

class GameEngine {
public:
    GameEngine();

    ~GameEngine();

    void startupPhase();

    void mainGameLoop();

    void reinforcementPhase();

    void issueOrdersPhase();

    void executeOrdersPhase();

    bool isExecutionDone() const;

    int getPlayerArmyCount(int numberOfPlayers) const;

    void initPlayers();

    /**
     * Assigns territories to players in a round-robin fashion.
     */
    void assignTerritories();

    /**
     * print territories with their owner names, grouped by continents
     */ 
    void printTerritoryOwners();

    Player* getWinningPlayer();

    friend class PhaseObserver;
    friend class GameStatisticsObserver;
private:
    vector<Player*> players;
    Map* map;
    Deck* deck;
    GameUI* gameUI;
};
