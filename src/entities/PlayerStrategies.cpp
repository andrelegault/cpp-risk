#include "PlayerStrategies.hpp"

/******************************************************
 * HUMAN STRATEGY
 *****************************************************/

void HumanPlayerStrategy::issueOrder(Player* player){
    
}

vector<Territory*>  HumanPlayerStrategy::toAttack(Player* player){
    set<Territory*> enemyTerritories;

    for (auto territory : player->getTerritories()) {
        for (auto border : territory->getBorders()) {
            Territory* neighbour = (Territory*)border->getOther(territory);

            if (neighbour != nullptr && neighbour->getOwner() != player) {
                enemyTerritories.insert(neighbour);
            }
        }
    }

    return vector<Territory*>(enemyTerritories.begin(), enemyTerritories.end());
}

vector<Territory*>  HumanPlayerStrategy::toDefend(Player* player){
    return player->getTerritories();
}

/******************************************************
 * AGGRESIVE STRATEGY
 *****************************************************/

void AgressivePlayerStrategy::issueOrder(Player* player){}

vector<Territory*>  AgressivePlayerStrategy::toAttack(Player* player){}

vector<Territory*>  AgressivePlayerStrategy::toDefend(Player* player){}

/******************************************************
 * BENEVOLENT STRATEGY
 *****************************************************/

void BenevolentPlayerStrategy::issueOrder(Player* player){}

vector<Territory*>  BenevolentPlayerStrategy::toAttack(Player* player){}

vector<Territory*>  BenevolentPlayerStrategy::toDefend(Player* player){}

/******************************************************
 * NEUTRAL PLAYER STRATEGY
 *****************************************************/

void NeutralPlayerStrategy::issueOrder(Player* player){}

vector<Territory*>  NeutralPlayerStrategy::toAttack(Player* player){}

vector<Territory*>  NeutralPlayerStrategy::toDefend(Player* playeu){}