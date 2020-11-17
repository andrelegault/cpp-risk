#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "Map.fwd.hpp"
#include "Player.hpp"

using namespace std;

/**
 * Bidirectional Graph collecting Territory nodes, Border edges, and Continent subgroups.
 * We can discuss if borders and territories should be handled by the subgroups.
 */
class Map {
public:
    Player* neutralP;

    /**
     * Default constructor.
     */
    Map();

    /**
     * Primary constructor.
     * @param name Name of Map.
     */
    Map(const string name);

    /**
     * Copy constructor.
     * @param map Map to copy.
     */
    Map(const Map& map);

    /**
     * Destructor.
     */
    ~Map();

    /**
     * Stream operator to describe Map in string format.
     * @param os The stream to output to.
     * @param map The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& stream, const Map& map);

    /**
     * Assignment operator overload.
     * @param other Other Map used for assignment.
     */
    Map& operator=(const Map& map);

    /**
     * Adds a continent to the map.
     * @param continent Continent to insert.
     */
    void connect(Continent* continent);

    /**
     * Finds equivalent Continent in Map (used for deep copy).
     *
     * @param continent Continent to find.
     * @return Equivalent Continent.
     */
    Continent* get(const Continent& continent);

    /**
     * Finds equivalent Territory in Map (used for deep copy).
     *
     * @param territory Territory to find.
     * @return Equivalent Territory.
     */
    Territory* get(const Territory& territory);

    /**
     * Finds equivalent Border in Map (used for deep copy).
     *
     * @param border Border to find.
     * @return Equivalent Border.
     */
    Border* get(const Border& border);

    /**
     * Removes a continent from the map.
     * @param continent Continent to remove.
     */
    void remove(Continent* continent);

    /**
     * Returns list of all Continents.
     */
    vector<Continent*> getContinents() const;

    /**
     * Returns list of all Territories.
     */
    vector<Territory*> getTerritories() const;

    /**
     * Returns list of all Borders.
     */
    vector<Border*> getBorders() const;

    /**
     * Method that checks if:
     * 1) The map is a connected graph
     * 2) Continents are connected subgraphs
     * 3) Each territory belongs to one contient.
     * @return The Map is valid.
     */
    bool validate();

private:
    // Collection of continents.
    vector<Continent*> continents;

    // Map name.
    string name;
};

ostream& operator<<(ostream& stream, const Map& map);

/**
 * Abstracted node for bidirectional graphs.
 */
class MapNode {
public:
    /**
     * Default constructor.
     */
    MapNode();

    /**
     * Primary constructor.
     * @param name Name of MapNode.
     */
    MapNode(const string name);

    /**
     * Copy constructor.
     * @param mapNode MapNode to copy.
     */
    MapNode(const MapNode& mapNode);

    /**
     * Destructor.
     */
    ~MapNode();

    /**
     * Name getter.
     */
    string getName();

    /**
     * Inserts the Border edge reference.
     * We expect the border to include self.
     *
     * @param border Border to attach to self.
     */
    void connect(Border* border);

    /**
     * Removes the Border edge reference.
     *
     * @param border Border to detach from self.
     */
    void remove(Border* border);

    /**
     * Connects other node to this node
     *
     * @param node Node to connect
     */
    void connect(MapNode* node);

    /**
     * Equality operator overload.
     * @param m1 MapNode to check.
     * @param m2 MapNode to compare against.
     * @return MapNodes are the same?
     */
    friend bool operator== (const MapNode& m1, const MapNode& m2);

    /**
     * Assignment operator.
     * @param other Reference to MapNode object used for assignment.
     * @return Reference to MapNode object.
     */
    MapNode& operator=(const MapNode& other);

    /**
     * Stream insertion operator.
     * @param stream Stream to output to.
     * @param node Node used for output.
     * @return Reference to stream containing output.
     */
    friend ostream& operator<<(ostream& stream, const MapNode& node);

    /**
     * Gets map object relative to this object.
     * @return Map.
     */
    virtual Map* getMap() const = 0;

    /**
     * Gets raw Borders.
     *
     * @return Borders.
     */
    vector<Border*> getBorders() const;

    

protected:
    // The name as defined in the Conquer Map file.
    string name;
    // Collection of Border edges.
    vector<Border*> borders;

    
};

bool operator== (const MapNode& m1, const MapNode& m2);

/**
 * A Node Subgroup for the Map graph.
 */
class Continent : public MapNode {
public:
    /// Default constructor.
    Continent();

    /**
     * Primary constructor.
     * @param name Name of Continent.
     * @param bonus Bonus for that continent.
     */
    Continent(string name, int bonus = 0);

    /**
     * Copy constructor.
     * @param continent Continent to copy.
     */
    Continent(const Continent& continent);

    /**
     * Destructor.
     */
    ~Continent();

    /**
     * Stream operator to describe Continent in string format.
     * @param os The stream to output to.
     * @param continent The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& stream, const Continent& continent);

    /**
     * Assignment operator overload.
     * @param other Other Continent used for assignment.
     */
    Continent& operator=(const Continent& continent);

    /**
     * Equality operator overload.
     * @param c1 Continent to check.
     * @param c2 Continent to compare against.
     * @return Continents are the same?
     */
    friend bool operator== (const Continent& c1, const Continent& c2);

    /**
     * Adds a territory to the territories list.
     * We expect the territory to stay in self for the duration of the game.
     *
     * @param territory Territory to add.
     */
    void connect(Territory* territory);

    /**
     * Sets parent map.
     *
     * @param map Parent to set to.
     */
    void connect(Map* map);

    /**
     * Removes Territory from territories.
     *
     * @param territory Territory to remove.
     */
    void remove(Territory* territory);

    /**
     * Get territories.
     *
     * @return Territories.
     */
    vector<Territory*> getTerritories() const;

    /**
     * Gets map object relative to this object.
     * @return Map.
     */
    Map* getMap() const;

    /// Gets the bonus for the continent.
    const int getBonus() const;

    /**
     * Checks if containing territories are connected.
     *
     * @return Are territories connected?
     */
    bool validate();

private:
    /// Territories inside continent subgroup.
    vector<Territory*> territories;
    /// Map that Continent is part of.
    Map* map;

    /// Bonus armies for the continent.
    int bonus;
};

bool operator== (const Continent& c1, const Continent& c2);
ostream& operator<<(ostream& stream, const Continent& continent);

// Territory

/**
 * Node for the Map graph.
 */
class Territory : public MapNode {
public:
    /**
     * Default constructor.
     */
    Territory();

    /**
     * Primary constructor.
     * @param name Name of Territory.
     */
    Territory(const string name);

    /**
     * Copy constructor.
     * @param mapNode Territory to copy.
     */
    Territory(const Territory& territory);

    /**
     * Destructor.
     */
    ~Territory();

    /**
     * Stream operator to describe Territory in string format.
     * @param os The stream to output to.
     * @param territory The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& stream, const Territory& territory);

    /**
     * Assignment operator overload.
     * @param other Other Territory used for assignment.
     */
    Territory& operator=(const Territory& territory);

    /**
     * Equality operator overload.
     * @param t1 Territory to check.
     * @param t2 Territory to compare against.
     * @return Territories are the same?
     */
    friend bool operator== (const Territory& t1, const Territory& t2);

    /**
     * Changes the ownership of the country to a certain Player.
     * We expect the country to be removed from the previous Player's list
     * and added to the current Player's list.
     *
     * @param player Player to make owner of.
     */
    void setPlayerOwner(Player* player);

    /**
     * return the name of the player who owns the territory
     */
    string getOwnerName();

    /**
     * Sets parent Continent.
     *
     * @param continent Continent to add.
     */
    void connect(Continent* continent);

    /**
     * Gets map object relative to this object.
     * @return Map.
     */
    Map* getMap() const;

    /// Gets the player owning this territory.
    Player* getOwner() const;

    /**
     * Validates that node is connected.
     *
     * @return Node is connected.
     */
    bool validate();

    /**
     * Returns vector of territory that borders this that is owned by player.
     */
    vector<Territory*> getPlayerBorderTerritories(Player* player);

    /**
     * Simulates an attack on a target territory using a specified number of units from this territory.
     *
     * The battle ends when one territory has no more armies.
     *
     * If the attacker wins, their leftover armies are transferred to the target territory and ownership is transferred.
     *
     * @param attacker Offensive player.
     * @param defender Defensive player.
     * @param attackerOdds Odds the attacker for each army (default 60).
     * @param defenderOdds Odds for each of the defender's army (default 70).
     * @return Whether the attacker was successful.
     */
    bool attack(Territory* target, int attackerArmies, int attackerOdds = 60, int defenderOdds = 70);

    /**
     * Creates a table string.
     */
    static std::string territoryTable(std::vector<Territory*> territories);

    void setNumberOfArmies(int numberOfArmies);

    int getNumberOfArmies();
private:
    // Counter for number of armies that current player owns in this country.
    int numberOfArmies;

    // Current player that owns this country.
    Player* playerOwner;

    // Parent Continent.
    Continent* continent;
};

bool operator==(const Territory& t1, const Territory& t2);
ostream& operator<<(ostream& stream, const Territory& territory);

// Border

/**
 * Bidirectional edge for the Map graph.
 */
class Border {
public:
    /**
     * Default constructor.
     */
    Border();

    /**
     * Primary constructor.
     * @param n1 MapNode connected to Border edge.
     * @param n2 MapNode connected to Border edge.
     */
    Border(MapNode* n1, MapNode* n2);

    /**
     * Copy constructor.
     * @param border Border to copy.
     */
    Border(const Border& border);

    /**
     * Destructor.
     */
    ~Border();

    /**
     * Stream operator to describe Border in string format.
     * @param os The stream to output to.
     * @param border The object to convert to string.
     * @return Updates ostream&.
     */
    friend ostream& operator<<(ostream& stream, const Border* border);

    /**
     * Equality operator overload.
     * @param b1 Border to check.
     * @param b2 Border to compare against.
     * @return Borders are the same?
     */
    friend bool operator== (const Border& b1, const Border& b2);

    /**
     * Assignment operator overload.
     * @param other Other Border used for assignment.
     */
    Border& operator=(const Border& border);

    /**
     * Fetches the other MapNode from the current node.
     *
     * @param self MapNode to find the other node from.
     * @return Other MapNode.
     */
    MapNode* getOther(MapNode* self);

    /**
     * Checks if MapNode is in current border.
     *
     * @param node MapNode to check.
     * @return Is MapNode part of the border.
     */
    bool has(MapNode* node);

    /**
     * Gets map object relative to this object.
     * @return Map.
     */
    Map* getMap() const;

private:
    // MapNode connected to Border edge.
    MapNode* n1;
    // MapNode connected to Border edge.
    MapNode* n2;
};

bool operator== (const Border& b1, const Border& b2);

ostream& operator<<(ostream& stream, const Border* border);
