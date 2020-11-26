#include "PlayerStrategies.hpp"

PlayerStrategy::~PlayerStrategy() {}

/******************************************************
 * HUMAN STRATEGY
 *****************************************************/

HumanPlayerStrategy::~HumanPlayerStrategy() {}

std::vector<std::string> territoryArmies(std::vector<std::pair<Territory*, int>> territoryArmies) {
    std::vector<std::string> strings;

    for (auto entry : territoryArmies) {
        strings.push_back(entry.first->getName() + " -> " + std::to_string(entry.second));
    }

    return strings;
}

void HumanPlayerStrategy::issueOrder(Player* player) {
    std::vector<std::pair<Territory*, int>> territoriesDefend;

    for (auto territory : player->toDefend()) {
        territoriesDefend.push_back({ territory, territory->getNumberOfArmies() });
    }

    std::vector<std::pair<Territory*, int>> territoriesAttack;

    for (auto territory : player->toAttack()) {
        territoriesAttack.push_back({ territory, territory->getNumberOfArmies() });
    }

    int armies = player->armies;

    while (armies > 0) {
        int entryIndex = UI::ask("Select territory to deploy to.", territoryArmies(territoriesDefend)) - 1;

        std::pair<Territory*, int> entry = territoriesDefend[entryIndex];

        Territory* territory = entry.first;

        int numberOfArmies = UI::range("Enter number of armies.", 0, armies);

        entry.second += numberOfArmies;

        territoriesDefend[entryIndex] = entry;

        armies -= numberOfArmies;

        player->addOrder(new Deploy(player, territory, numberOfArmies));
    }

    while (true) {
        Territory* target;

        switch (UI::ask("What would you like to do?", { "Attack", "Defend", "End" })) {
        case 1:
            target = player->toAttack()[UI::ask("Which territory to attack?", territoryArmies(territoriesAttack)) - 1];

            break;
        case 2:
            target = player->toDefend()[UI::ask("Which territory to defend?", territoryArmies(territoriesDefend)) - 1];

            break;
        default:
            goto nextState;
        }

        std::vector<std::pair<Territory*, int>> territoriesAdjacent;

        std::vector<int> adjacentIndicies;

        for (auto territory : target->getPlayerBorderTerritories(player)) {
            for (auto [entry, index] : Utils::enumerate(territoriesDefend)) {
                if (entry.first == territory) {
                    territoriesAdjacent.push_back({ territory, entry.second });
                    adjacentIndicies.push_back(index);

                    break;
                }
            }
        }

        if (adjacentIndicies.size() == 0) continue;

        int adjacentIndex = UI::ask("Which source territory?", territoryArmies(territoriesAdjacent)) - 1;

        std::pair<Territory*, int> sourceArmy = territoriesAdjacent[adjacentIndex];

        Territory* source = sourceArmy.first;

        int armyCount = UI::range("How many armies?", 0, sourceArmy.second);

        territoriesDefend[adjacentIndicies[adjacentIndex]].second -= armyCount;

        player->addOrder(new Advance(player, source, target, armyCount));
    }

nextState:;
    if (player->hand->getLength() == 0) return;

    map<CardType, Card*> cardTypeMap;

    for (auto card : player->hand->getCards()) {
        cardTypeMap[*(card->cardType)] = card;
    }

    vector<CardType> cardTypeVector;

    std::transform(cardTypeMap.begin(), cardTypeMap.end(), std::back_inserter(cardTypeVector), [](auto& pair) { return pair.first; });

    vector<string> cardTypeStrings;

    for (auto cardType : cardTypeVector) {
        cardTypeStrings.push_back(cardTypeToString(cardType));
    }

    int cardIndex = UI::ask("Choose a card to play.", cardTypeStrings) - 1;

    Card* card = cardTypeMap[cardTypeVector[cardIndex]];

    // TODO: UI for playing card.

    //card->play(player);
}

vector<Territory*> HumanPlayerStrategy::toAttack(Player* player) {
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

vector<Territory*> HumanPlayerStrategy::toDefend(Player* player) {
    return player->getTerritories();
}

/******************************************************
 * AGGRESIVE STRATEGY
 *****************************************************/

void AggressivePlayerStrategy::issueOrder(Player* player) {
    Territory* source = player->toDefend()[0];

    player->addOrder(new Deploy(player, source, player->armies));

    int armies = player->armies + source->getNumberOfArmies();

    if (player->toAttack().size() == 0) return;

    int armyCount = armies / player->toAttack().size();

    for (auto target : player->toAttack()) {
        player->addOrder(new Advance(player, source, target, armyCount));
    }

    // TODO?: fortifies in order to maximize aggregation of forces in one country.
}

vector<Territory*> AggressivePlayerStrategy::toAttack(Player* player) {
    set<Territory*> enemyTerritories;

    for (auto territory : player->toDefend()) {
        for (auto border : territory->getBorders()) {
            Territory* neighbour = (Territory*)border->getOther(territory);

            if (neighbour != nullptr && neighbour->getOwner() != player) {
                enemyTerritories.insert(neighbour);
            }
        }
    }

    return vector<Territory*>(enemyTerritories.begin(), enemyTerritories.end());
}

vector<Territory*> AggressivePlayerStrategy::toDefend(Player* player) {
    Territory* maxTerritory = nullptr;

    for (auto territory : player->getTerritories()) {
        if (maxTerritory == nullptr || maxTerritory->getNumberOfArmies() < territory->getNumberOfArmies()) {
            maxTerritory = territory;
        }
    }

    if (maxTerritory == nullptr) throw runtime_error("Agressive player has no territory to defend.");

    return { maxTerritory };
}

/******************************************************
 * BENEVOLENT STRATEGY
 *****************************************************/

void BenevolentPlayerStrategy::issueOrder(Player* player) {}

vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* player) {}

vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) {}

/******************************************************
 * NEUTRAL PLAYER STRATEGY
 *****************************************************/

void NeutralPlayerStrategy::issueOrder(Player* player) {}

vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) {}

vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) {}