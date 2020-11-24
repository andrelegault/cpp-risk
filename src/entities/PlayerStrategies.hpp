#pragma once

#include "PlayerStrategies.fwd.cpp"
#include "Map.hpp"
#include "Player.hpp"
#include "UI.hpp"
#include "Order.hpp"
#include "Utils.hpp"

using namespace std;

class PlayerStrategy{
public:
    ~PlayerStrategy();
    virtual void issueOrder(Player* player) = 0;
    virtual vector<Territory*> toAttack(Player* player) = 0;
    virtual vector<Territory*> toDefend(Player* player) = 0;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    ~HumanPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    ~AggressivePlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    ~BenevolentPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    ~NeutralPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
};