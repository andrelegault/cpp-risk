#ifndef country_H
#define country_H
#endif

/**
 * Bidirectional edge for the Map graph.
 */
class Border {
    private:
        // Country node connected to Border edge.
        Country c1;
        // Country node connected to Border edge.
        Country c2;

    public:
        Border(Country c1, Country c2);
        
        /**
         * Fetches the other Country node from the current country.
         * 
         * @param country Country node to find the other node from.
         * @return Other country node.
         */
        Country get_other_country(Country country);
};