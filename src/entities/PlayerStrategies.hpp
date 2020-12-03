#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>

#include "PlayerStrategies.fwd.cpp"
#include "Map.hpp"
#include "Player.hpp"
#include "UI.hpp"
#include "Order.hpp"
#include "Utils.hpp"

using namespace std;

class PlayerStrategy {
public:
    /// default constructor
    PlayerStrategy();
    /// copy constructor
    PlayerStrategy(const PlayerStrategy& other);
    /// destructor
    ~PlayerStrategy();
    /// creates a PlayerStrategy object depending on a provided integer
    static PlayerStrategy* fromInt(int random);
    /// the number of supported player strategies
    static const int numStrategies;
    /// issues an order
    virtual void issueOrder(Player* player) = 0;
    /// gets a list of territory pointers to attack
    virtual vector<Territory*> toAttack(Player* player) = 0;
    /// gets a list of territory pointers to defend
    virtual vector<Territory*> toDefend(Player* player) = 0;
    /// returns a string representation of the player strategy child class
    virtual string toString() const = 0;
    friend ostream& operator<<(ostream& stream, const PlayerStrategy& strategy);
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy();
    HumanPlayerStrategy(const HumanPlayerStrategy& other);
    ~HumanPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
    string toString() const;
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);
    friend ostream& operator<<(ostream& stream, const HumanPlayerStrategy& strategy);
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    ~AggressivePlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
    string toString() const;
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);
    friend ostream& operator<<(ostream& stream, const AggressivePlayerStrategy& strategy);
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy();
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    ~BenevolentPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
    string toString() const;
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);
    friend ostream& operator<<(ostream& stream, const BenevolentPlayerStrategy& strategy);
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    ~NeutralPlayerStrategy();
    void issueOrder(Player* player);
    vector<Territory*> toAttack(Player* player);
    vector<Territory*> toDefend(Player* player);
    string toString() const;
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);
    friend ostream& operator<<(ostream& stream, const NeutralPlayerStrategy& strategy);
};

ostream& operator<<(ostream& stream, const PlayerStrategy& strategy);
ostream& operator<<(ostream& stream, const HumanPlayerStrategy& strategy);
ostream& operator<<(ostream& stream, const AggressivePlayerStrategy& strategy);
ostream& operator<<(ostream& stream, const BenevolentPlayerStrategy& strategy);
ostream& operator<<(ostream& stream, const NeutralPlayerStrategy& strategy);

