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
#include "PlayerStrategies.hpp"
#include "Utils.hpp"
#include "MapLoader.hpp"
#include "Map.hpp"
#include "Cards.hpp"
#include "GameObservers.hpp"

using namespace filesystem;
using namespace UI;

/**
 * Enum for states (phases) that the GameEngine can be in.
 */
enum GamePhase {
    STARTUP_PHASE,
    REINFORCEMENT_PHASE,
    ISSUE_ORDER_PHASE,
    EXECUTE_ORDER_PHASE
};

/**
 * Converts the GamePhase enum to string.
 *
 * @param gamePhase The enum to convert.
 * @return The string version of the enum.
 */
string gamePhaseToString(GamePhase gamePhase);

class GameEngine : public Subject {
public:
    /**
     * The first player of the tuple is immune to the attacks of the second.
     */
    static map<tuple<Player*, Player*>, bool> immunities;

    /// default constructor
    GameEngine();

    /// destructor
    ~GameEngine();

    /**
     * The entrypoint for the GameEngine. Should be called to start the game.
     */
    void init();

    /**
     * Shuffles the players and sets their initial armyCount.
     */
    void initPlayers();

    /**
     * Startup Phase.
     *
     * - Orders players randomly.
     * - Territories are assigned randomly.
     * - Players are assigned initial armies as described in rules.
     */
    void startupPhase();

    /**
     * Loop that englobes all recurring GamePhases.
     */
    void mainGameLoop();

    /**
     * Reinforcement Phase.
     *
     * - Give players number of armies according to terrtiory/continent owned.
     */
    void reinforcementPhase();

    /**
     * Issue Order Phase.
     *
     * - Deploys armies according to number held by player.
     * - Attacks/defends territories.
     */
    void issueOrdersPhase();

    /**
     * Execute Order Phase.
     *
     * - Executes orders from issueOrder.
     *
     * Should maybe be the one handling win state?
     */
    void executeOrdersPhase();

    /**
     * Checks if players still have orders to play.
     *
     * @return Players don't have orders?
     */
    bool isExecutionDone() const;

    /**
     * Returns the armyCount for each player according to player side.
     *
     * @param numberOfPlayers
     * @return Number of armies.
     */
    int getPlayerArmyCount(int numberOfPlayers) const;

    /**
     * Assigns territories to players in a round-robin fashion.
     */
    void assignTerritories();

    /**
     * Getter for Current Player.
     * @return Current Player.
     */
    Player* getCurrentPlayer();

    /**
     * Getter for GamePhase.
     * @return GamePhase.
     */
    GamePhase getGamePhase();

    friend class PhaseObserver;

    friend class GameStatisticsObserver;

    friend ostream& operator<<(ostream& stream, const GameEngine& gameEngine);
private:
    vector<Player*> players;
    Player* currentPlayer;
    GamePhase gamePhase;
    Map* warzoneMap;
    Deck* deck;
    GameUI* gameUI;

    /**
     * GamePhase setter.
     * @param gamePhase
     */
    void setGamePhase(GamePhase gamePhase);

    /**
     * CurrentPlayer setter.
     * @param player
     */
    void setCurrentPlayer(Player* player);
};

ostream& operator<<(ostream& stream, const GameEngine& gameEngine);
