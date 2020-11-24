#pragma once

#include "PlayerStrategies.fwd.hpp"
#include "Map.hpp"
#include "Player.hpp"

using namespace std;

class PlayerStrategy{
public:
    virtual ~PlayerStrategy();
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

class AgressivePlayerStrategy : public PlayerStrategy {
public:
    ~AgressivePlayerStrategy();
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