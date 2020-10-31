#pragma once

#include <vector>
#include "Observer.hpp"

using namespace std;

/**
 * MVC Model component.
 */
class Subject {
    public:
        /**
         * Attaches an observer to subject for update.
         */
        void attach(Observer* observer);

        /**
         * Attaches many observer to subject.
         */
        void attach(vector<Observer*> observers);

        /**
         * Detaches an observer from subject.
         */
        void detach(Observer* observer);

    protected:
        /**
         * Notifies the attached observers.
         */
        void notify();

    private:
        /**
         * Observers attached to subject.
         */
        vector<Observer*> observers;
};