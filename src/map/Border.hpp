#pragma once

#include <iostream>

#include <Border.fwd.hpp>
#include <Map.hpp>

using namespace std;

/**
 * Bidirectional edge for the Map graph.
 */
class Border {
private:
    // MapNode connected to Border edge.
    MapNode* n1;
    // MapNode connected to Border edge.
    MapNode* n2;

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
    Border(Border* border);

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
    void operator=(const Border* border);

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
};

bool operator== (const Border& b1, const Border& b2);

ostream& operator<<(ostream& stream, const Border* border);