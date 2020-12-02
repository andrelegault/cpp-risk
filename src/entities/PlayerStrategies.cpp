#include "PlayerStrategies.hpp"

PlayerStrategy::~PlayerStrategy() {}

ostream& operator<<(ostream& stream, const PlayerStrategy& playerStrategy) {
    stream << playerStrategy.toString();
    return stream;
}

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
        // i think there's a bug here, whenever i enter a number smaller than the max, the max is always chosen

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

    CardType cardType = cardTypeVector[cardIndex];

    Card* card = cardTypeMap[cardType];

    Territory* source = nullptr;
    Territory* target = nullptr;
    Player* targetPlayer = nullptr;
    std::pair<Territory*, int> territoryPair;
    int armyCount = 0;

    switch (cardType) {
    case CardType::AIRLIFT:
        territoryPair = territoriesDefend[UI::ask("Which source territory?", territoryArmies(territoriesDefend)) - 1];

        source = territoryPair.first;

        armyCount = UI::range("How many armies?", 0, territoryPair.second);

        target = territoriesDefend[UI::ask("Which target territory?", territoryArmies(territoriesDefend)) - 1].first;

        break;
    case CardType::REINFORCEMENT:
    case CardType::BLOCKADE:
        source = territoriesDefend[UI::ask("Which territory?", territoryArmies(territoriesDefend)) - 1].first;

        break;
    case CardType::BOMB:
        target = territoriesAttack[UI::ask("Which territory?", territoryArmies(territoriesAttack)) - 1].first;

        break;
    case CardType::DIPLOMACY:
        break;
    }

    if (cardType == CardType::DIPLOMACY) {
        std::set<Player*> enemySet;

        for (auto territory : player->getTerritories()) {
            enemySet.insert(territory->getOwner());
        }

        std::vector<Player*> enemies;
        std::vector<string> enemyNames;

        for (auto enemy : enemySet) {
            enemies.push_back(enemy);
            enemyNames.push_back(enemy->getName());
        }

        targetPlayer = enemies[UI::ask("Which player?", enemyNames) - 1];
    }

    card->play(player, targetPlayer, source, target, armyCount);
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

string HumanPlayerStrategy::toString() const {
    return "Human";
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

string AggressivePlayerStrategy::toString() const {
    return "Aggressive";
}


/******************************************************
 * BENEVOLENT STRATEGY
 *****************************************************/

void BenevolentPlayerStrategy::issueOrder(Player* player) {
    // reinforces weakest territories first
    // never attacks therefore will never have cards
    vector<Territory*> ascending = toDefend(player);

    const int numberToDefend = ascending.size();
    if (!numberToDefend) return;
    player->addOrder(new Deploy(player, ascending.back(), player->armies));
    if (numberToDefend == 1) return;

    // issue defensive advance/airlift orders to fortify weaker territories
    Territory* weakest = ascending.back();
    Territory* strongest = ascending.front();

    const bool useAdvance = weakest->isNeighbour(strongest);

    const int difference = abs(strongest->getNumberOfArmies() - weakest->getNumberOfArmies());
    if (!difference) return;

    const int armies = difference / 2;
    Order* fortify = nullptr;
    if (useAdvance) fortify = new Advance(player, strongest, weakest, armies);
    else fortify = new Airlift(player, strongest, weakest, armies);
    player->addOrder(fortify);
}

vector<Territory*> BenevolentPlayerStrategy::toAttack(Player* player) { return {}; }

vector<Territory*> BenevolentPlayerStrategy::toDefend(Player* player) {
    // sort territories in ascending order (according to numberOfArmies)
    // a copy is made to not alter the player's territory vector
    vector<Territory*> copyToSort = player->getTerritories();
    sort(copyToSort.begin(), copyToSort.end());
    return copyToSort;
}

string BenevolentPlayerStrategy::toString() const {
    return "Benevolent";
}

/******************************************************
 * NEUTRAL PLAYER STRATEGY
 *****************************************************/

void NeutralPlayerStrategy::issueOrder(Player* player) {}

vector<Territory*> NeutralPlayerStrategy::toAttack(Player* player) { return {}; }

vector<Territory*> NeutralPlayerStrategy::toDefend(Player* player) { return {}; }

string NeutralPlayerStrategy::toString() const {
    return "Neutral";
}