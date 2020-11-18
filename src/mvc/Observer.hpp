#pragma once

#include "Observer.fwd.hpp"

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