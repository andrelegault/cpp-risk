#pragma once

#include "Text.fwd.hpp"
#include "UI.hpp"

std::ostream& operator<<(std::ostream& stream, const UI::Text& text);

/**
 * A UI text (alot like HTML p, span, h1.., etc)
 */
class UI::Text : public UI::Component {
public:
    std::string text;

    Text();

    ~Text();

    Text(std::string text, const UI::Style& style = {});

    Text(const UI::Text& text);

    friend std::ostream& ::operator<<(std::ostream& stream, const UI::Text& text);

    virtual std::ostream& toString(std::ostream& stream) const override { stream << *this; return stream; };

    virtual UI::Component* clone() const override { return new Text(*this); };
};
