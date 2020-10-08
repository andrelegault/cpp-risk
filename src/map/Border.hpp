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
    // MapNode node connected to Border edge.
    MapNode* n2;

public:
    Border(MapNode* n1, MapNode* n2);
    Border(Border* border);
    ~Border();

    friend ostream& operator<<(ostream& stream, const Border* border);

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
};

ostream& operator<<(ostream& stream, const Border* border);