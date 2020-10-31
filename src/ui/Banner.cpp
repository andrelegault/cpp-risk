#include "Banner.hpp"

UI::Banner::~Banner() {};

UI::Banner::Banner(UI::Style style) : UI::Component(style) {};

UI::Banner::Banner(const UI::Banner& banner) : UI::Banner(banner.style) {};

std::ostream& operator<<(std::ostream& stream, const UI::Banner& banner) {
    stream << " __    __   ____  ____   _____   ___   ____     ___ " << std::endl;
    stream << "|  |__|  | /    ||    \\ |     | /   \\ |    \\   /  _]" << std::endl;
    stream << "|  |  |  ||  o  ||  D  )|__/  ||     ||  _  | /  [_ " << std::endl;
    stream << "|  |  |  ||     ||    / |   __||  O  ||  |  ||    _]" << std::endl;
    stream << "|  `  '  ||  _  ||    \\ |  /  ||     ||  |  ||   [_ " << std::endl;
    stream << " \\      / |  |  ||  .  \\|     ||     ||  |  ||     |" << std::endl;
    stream << "  \\_/\\_/  |__|__||__|\\_||_____| \\___/ |__|__||_____|" << std::endl;

    return stream;
}