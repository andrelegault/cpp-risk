#pragma once

#include "Grid.fwd.hpp"
#include "UI.hpp"
#include <sstream>

std::ostream& operator<<(std::ostream& stream, const UI::Grid& grid);

/**
 * A UI grid (alot like HTML table).
 */
class UI::Grid : public UI::Component {
public:
    Grid();

    ~Grid();

    Grid(UI::Component* component, UI::Style style = {});

    Grid(std::vector<std::vector<UI::Component*>> components, UI::Style style = {});

    Grid(const UI::Grid& grid);

    friend std::ostream& ::operator<<(std::ostream& stream, const Grid& grid);

    virtual std::ostream& toString(std::ostream& stream) const override { stream << *this; return stream; };

    virtual UI::Component* clone() const override { return new Grid(*this); };
private:
    std::vector<std::vector<UI::Component*>> components;
};