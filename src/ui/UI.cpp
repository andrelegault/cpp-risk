#include "UI.hpp"

void UI::clear() {
    std::cout << "\033[2J";
}

int UI::ask(const std::string& prompt, std::vector<std::string> options) {
    UI::clear();

    std::cout << UI::Grid({
        {new UI::Text(prompt)},
        {new UI::List(options, {.padding = 0, .border = false, .enumerate = true })}
        });

    int res = UI::validate(1, options.size());

    if (res >= 0) return res;

    return UI::ask(prompt, options);
}

int UI::ask(const UI::Component& component, std::vector<std::string> options) {
    UI::clear();

    std::stringstream ss;

    ss << component;

    return UI::ask(ss.str(), options);
}

int UI::validate(int min, int max) {
    int option = 0;

    std::cout << endl << "> ";

    if (cin >> option) {
        if (option >= min && option <= max) {
            return option;
        }
    }

    UI::clear();

    std::cout << UI::Grid(new UI::Text("Please enter a valid option."), {});

    std::cout << endl;

    return -1;
}

int UI::range(std::string prompt, int min, int max) {
    UI::clear();

    std::stringstream ss;

    ss << "(" << min << "," << max << ")";

    std::cout << UI::Grid({
        {new UI::Text(prompt)},
        {new UI::Text(ss.str())}
        });

    int res = UI::validate(min, max);

    if (res >= 0) return res;

    return UI::range(prompt, min, max);
}

std::vector<UI::Component*> UI::toText(std::vector<std::string> strings) {
    std::vector<UI::Component*> texts;

    for (auto string : strings) {
        texts.push_back(new UI::Text(string));
    }

    return texts;
}