#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

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

enum GamePhase {
    STARTUP_PHASE,
    REINFORCEMENT_PHASE,
    ISSUE_ORDER_PHASE,
    EXECUTE_ORDER_PHASE
};

string gamePhaseToString(GamePhase gamePhase);

class GameEngine : public Subject {
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

    void init();

    /**
     * Assigns territories to players in a round-robin fashion.
     */
    void assignTerritories();

    /**
     * print territories with their owner names, grouped by continents
     */
    void printTerritories();

    Player* getWinningPlayer();

    Player* getCurrentPlayer();

    GamePhase getGamePhase();

    friend class PhaseObserver;
    friend class GameStatisticsObserver;

private:
    vector<Player*> players;
    Player* currentPlayer;
    GamePhase gamePhase;
    Map* map;
    Deck* deck;
    GameUI* gameUI;

    void setGamePhase(GamePhase gamePhase);

    void setCurrentPlayer(Player* player);
};
