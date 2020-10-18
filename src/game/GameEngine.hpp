#pragma once

#include <UI.hpp>
#include <Player.hpp>
#include <Utils.hpp>
#include <MapLoader.hpp>
#include <Map.hpp>
#include <Cards.hpp>
#include <filesystem>

using namespace filesystem;
using namespace UI;

class GameEngine {
private:
    vector<Player*> players;
    Map* map;
    Deck* deck;

public:
    GameEngine();

    void startupPhase();

    void mainGameLoop();

    void reinforcementPhase();

    void issueOrdersPhase();

    void executeOrdersPhase();
};