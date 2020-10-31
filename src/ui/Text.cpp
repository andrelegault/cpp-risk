#include "Text.hpp"

UI::Text::Text() : Text("", {}) {};

UI::Text::~Text() {};

UI::Text::Text(std::string text, const UI::Style& style) : text(text), UI::Component(style) {};

UI::Text::Text(const UI::Text& text) : Text(text.text, text.style) {};

std::ostream& operator<<(std::ostream& stream, const UI::Text& text) {
    stream << text.text;

    return stream;
}