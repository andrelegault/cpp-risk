#include "Viewport.hpp"

UI::Viewport::Viewport() : Viewport(0, {}) {};

UI::Viewport::~Viewport() {
    if (this->component) {
        delete this->component;
    }
};

UI::Viewport::Viewport(UI::Component* component, UI::Style style) : component(component), UI::Component(style) {
    this->component->parent = this;
};

UI::Viewport::Viewport(const Viewport& viewport) : Viewport(viewport.component->clone(), viewport.style) {};

std::ostream& operator<<(std::ostream& stream, const UI::Viewport& viewport) {
    std::stringstream ss;

    ss << *viewport.component;

    std::vector<std::string> lines = Utils::split(ss.str(), "\n");

    for (int i = viewport.style.y; i < viewport.style.y + viewport.style.height; i++) {
        if (i >= lines.size() || i < 0) {
            stream << std::string(viewport.style.width, ' ') << std::endl;
        }
        else {
            std::string line = lines[i];

            std::string frontPad = std::string(std::min(std::max(-viewport.style.x, 0), viewport.style.width), ' ');

            line = line.substr(
                std::min(
                    std::max(viewport.style.x, 0),
                    (int)line.size()
                ),
                std::max(viewport.style.width - std::max(-viewport.style.x, 0), 0)
            );

            std::string backPad = std::string(viewport.style.width - (int)frontPad.size() - (int)line.size(), ' ');

            stream << frontPad << line << backPad << std::endl;
        }
    }

    return stream;
}