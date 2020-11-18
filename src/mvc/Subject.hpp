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
         * 
         * @param observer Observer to attach.
         */
        void attach(Observer* observer);

        /**
         * Attaches many observer to subject.
         * 
         * @param observers Observers to attach.
         */
        void attach(vector<Observer*> &observers);

        /**
         * Detaches an observer from subject.
         * 
         * @param observer Observer to detach.
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