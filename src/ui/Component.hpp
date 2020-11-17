#pragma once

#include <string>
#include <iostream>

#include "Component.fwd.hpp"
#include "Style.hpp"

std::ostream& operator<<(std::ostream& stream, const UI::Component& component);

namespace UI {
    /**
     * Generic UI Component.
     */
    class Component {
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

        friend std::ostream& ::operator<<(std::ostream& stream, const Component& component);

        /**
         * Used for virtual operator<<.
         */
        virtual std::ostream& print(std::ostream& stream) const = 0;

        /**
         * Used for virtual clone constructor.
         */
        virtual UI::Component* clone() const = 0;

        /**
         * Updates the component (and it's parent hierachy).
         */
        void update();
    };
}
