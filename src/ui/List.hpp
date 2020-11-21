#pragma once

#include "List.fwd.hpp"
#include "UI.hpp"
#include <sstream>

std::ostream& operator<<(std::ostream& stream, const UI::List& list);

/**
 * A UI list (alot like HTML ul/l).
 */
class UI::List : public UI::Component {
public:
    List();

    ~List();

    List(std::vector<UI::Component*> components, UI::Style style = { .padding = 0, .border = false });

    List(std::vector<std::string> strings, UI::Style style = { .padding = 0, .border = false });

    List(const List& list);

    friend std::ostream& ::operator<<(std::ostream& stream, const List& list);

    virtual std::ostream& toString(std::ostream& stream) const override { stream << *this; return stream; };

    virtual UI::Component* clone() const override { return new List(*this); };
private:
    std::vector<UI::Component*> components;
};
