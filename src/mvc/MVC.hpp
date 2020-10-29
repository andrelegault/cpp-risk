#pragma once

#include <vector>
#include "MVC.fwd.hpp"

using namespace std;

/**
 * MVC View component.
 */
class Observer {
    public:
        /**
         * Friending Subject to make only subjects be able to modify observer.
         */
        friend class Subject;

    protected:
        /**
         * Updates the observer.
         * 
         * @param subject The subject that is updated.
         */
        virtual void update() = 0;
};

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