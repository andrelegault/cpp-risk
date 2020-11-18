#pragma once

#include <string>
#include <iostream>

#include "Component.fwd.hpp"
#include "Style.hpp"

std::ostream& operator<<(std::ostream& stream, const UI::Component& component);

/**
 * Generic UI Component.
 */
class UI::Component {
public:
    /**
     * The component style.
     */
    UI::Style style;

    UI::Component* parent;

    Component();

    ~Component();

    Component(Style style);

    Component(const Component& component);

    /**
     * Virtual operator<<.
     */
    virtual std::ostream& toString(std::ostream& stream) const = 0;

    /**
     * Virtual clone constructor.
     */
    virtual UI::Component* clone() const = 0;

    /**
     * Updates component (and parent hierachy).
     */
    void update();

    friend std::ostream& ::operator<<(std::ostream& stream, const Component& component);
};