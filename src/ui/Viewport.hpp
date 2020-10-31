#pragma once

#include "Viewport.fwd.hpp"
#include "UI.hpp"

std::ostream& operator<<(std::ostream& stream, const UI::Viewport& viewport);

namespace UI {
    /**
     * A UI viewing component (alot like HTML div).
     */
    class Viewport : public UI::Component {
    public:
        Viewport();

        ~Viewport();

        Viewport(UI::Component* component, UI::Style style = {});

        Viewport(const UI::Viewport& viewport);

        friend std::ostream& ::operator<<(std::ostream& stream, const UI::Viewport& viewport);

        virtual std::ostream& print(std::ostream& stream) const override { stream << *this; return stream; };

        virtual UI::Component* clone() const override { return new UI::Viewport(*this); };
    private:
        UI::Component* component;
    };
}