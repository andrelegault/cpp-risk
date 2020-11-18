#pragma once

#include "Banner.fwd.hpp"
#include "UI.hpp"

std::ostream& operator<<(std::ostream& stream, const UI::Banner& banner);

/**
 * A UI component to display the game banner (could be generalized to create a large ASCII font).
 */
class UI::Banner : public UI::Component {
public:
    ~Banner();

    Banner(UI::Style style = {});

    Banner(const Banner& banner);

    friend std::ostream& ::operator<<(std::ostream& stream, const Banner& banner);

    virtual std::ostream& toString(std::ostream& stream) const override { stream << *this; return stream; };

    virtual UI::Component* clone() const override { return new Banner(*this); };
};