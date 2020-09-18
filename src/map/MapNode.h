#pragma once

#include <Border.h>
#include <Map.h>

#include <string>
#include <vector>

using namespace std;

/**
 * Abstracted node for bidirectional graphs.
 */
template <typename T>
class MapNode {
    protected:
        // The name as defined in the Conquer Map file.
        string name;
        // Collection of Border edges.
        vector<Border<MapNode<T>>*> borders;
        // Parent of this node type.
        T parent;
    public:
        MapNode(string name);
        ~MapNode();

        string getName();

        /**
         * Inserts the Border edge reference.
         * We expect the border to include self.
         * 
         * @param border Border to attach to self.
         */
        void add(Border<MapNode<T>>* border);

        /**
         * Removes the Border edge reference.
         * 
         * @param border Border to detach from self.
         */
        void remove(Border<MapNode<T>>* border);

        /**
         * Connects other node to this node
         * 
         * @param node Node to connect
         */
        void connect(MapNode<T>* node);

        /**
         * Sets parent.
         * 
         * @param parent Parent to set.
         */
        void set(T* parent);
};