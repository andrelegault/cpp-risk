#ifndef Territory_H
#define Territory_H
#endif

/**
 * Bidirectional edge for the Map graph.
 */
class Border {
    private:
        // Territory node connected to Border edge.
        Territory* t1;
        // Territory node connected to Border edge.
        Territory* t2;

    public:
        Border(Territory* t1, Territory* t2);
        
        /**
         * Fetches the other Territory node from the current territory.
         * 
         * @param territory Territory node to find the other node from.
         * @return Other territory node.
         */
        Territory* getOtherTerritory(Territory* territory);
};