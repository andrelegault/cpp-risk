#pragma once

#include <iostream>

using namespace std;

/**
 * Bidirectional edge for the Map graph.
 */
template <typename T>
class Border {
    private:
        // MapNode connected to Border edge.
        T* n1;
        // MapNode node connected to Border edge.
        T* n2;

    public:
        Border(T* n1, T* n2);
        Border(Border* border);
        ~Border();

        friend ostream& operator<<(ostream& stream, const Border<T>* border);
        
        /**
         * Fetches the other MapNode from the current node.
         * 
         * @param self MapNode to find the other node from.
         * @return Other MapNode.
         */
        T* getOther(T* self);
};

template <typename T>
ostream& operator<<(ostream& stream, const Border<T>* border);