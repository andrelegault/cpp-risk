#pragma once

#include <string>
#include <vector>
#include <MapNode.fwd.hpp>
#include <Border.hpp>

using namespace std;

/**
 * Abstracted node for bidirectional graphs.
 */
class MapNode {
protected:
    // The name as defined in the Conquer Map file.
    string name;
    // Collection of Border edges.
    vector<Border*> borders;
public:
    MapNode(string name);
    MapNode(MapNode* mapNode);
    ~MapNode();

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
};