#pragma once

#include "Map.hpp"
#include "Player.hpp"

using namespace std;

class PlayerStrategy{
public:    
    virtual ~PlayerStrategy();
    virtual void issueOrder() = 0;
    virtual vector<Territory*> toAttack() = 0;
    virtual vector<Territory*> toDefend() = 0;

};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    ~HumanPlayerStrategy();
    void issueOrder();
    vector<Territory*>  toAttack();
    vector<Territory*>  toDefend();
  
};

class AgressivePlayerStrategy : public PlayerStrategy {
public:
    ~AgressivePlayerStrategy();
    void issueOrder();
    vector<Territory*>  toAttack();
    vector<Territory*>  toDefend();
  
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    ~BenevolentPlayerStrategy();
    void issueOrder();
    vector<Territory*>  toAttack();
    vector<Territory*>  toDefend();
  
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    ~NeutralPlayerStrategy();
    void issueOrder();
    vector<Territory*>  toAttack();
    vector<Territory*>  toDefend();
  
};