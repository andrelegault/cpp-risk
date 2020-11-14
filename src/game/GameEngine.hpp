#pragma once

#include "GameEngine.fwd.hpp"
#include "UI.hpp"
#include "Player.hpp"
#include "Utils.hpp"
#include "MapLoader.hpp"
#include "Map.hpp"
#include "Cards.hpp"
#include "GameObservers.hpp"
#include <filesystem>

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

    int getPlayerArmyCount(int numberOfPlayers) const;

    Player* getWinningPlayer();

    friend class PhaseObserver;
    friend class GameStatisticsObserver;
private:
    vector<Player*> players;
    Map* map;
    Deck* deck;
    GameUI* gameUI;
};