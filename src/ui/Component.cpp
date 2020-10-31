#include "Component.hpp"

UI::Component::Component() : Component(UI::Style{}) {};

UI::Component::~Component() {};

UI::Component::Component(UI::Style style) : style(style) {};

UI::Component::Component(const Component& component) : Component(component.style) {};

std::ostream& operator<<(std::ostream& stream, const UI::Component& component) {
    return component.print(stream);
}

void UI::Component::update() {
    if (this->parent == nullptr) {
        std::cout << "\033[2J" << *this;
    }
    else {
        this->parent->update();
    }
}