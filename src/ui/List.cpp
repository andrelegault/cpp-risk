#include "List.hpp"

UI::List::List() : List(std::vector<std::string>{}, {}) {};

UI::List::~List() {
    while (!this->components.empty()) delete this->components.back();
};

UI::List::List(std::vector<UI::Component*> components, UI::Style style) : components(components), UI::Component(style) {
    for (auto component : this->components) {
        component->parent = this;
    }
};

UI::List::List(std::vector<std::string> strings, UI::Style style) : List(UI::toText(strings), style) {};

UI::List::List(const UI::List& list) {
    std::vector<UI::Component*> componentsClone;

    for (auto component : list.components) {
        componentsClone.push_back(component->clone());
    }

    UI::List(componentsClone, list.style);
}

std::ostream& operator<<(std::ostream& stream, const UI::List& list) {
    std::stringstream ss;

    int c = 1;

    for (auto component : list.components) {
        if (list.style.enumerate) {
            ss << c++ << ". " << *component << std::endl;
        }
        else {
            ss << "- " << *component << std::endl;
        }
    }

    stream << UI::Grid(new UI::Text(ss.str()), list.style);

    return stream;
}