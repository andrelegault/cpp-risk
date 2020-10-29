#include <UI.hpp>

using namespace UI;

/**
 * Style
 */

 /**
  * Component
  */
UI::Component::Component() : Component(Style{}) {};

UI::Component::~Component() {};

UI::Component::Component(Style style) : style(style) {};

UI::Component::Component(const Component& component) : Component(component.style) {};

ostream& operator<<(ostream& stream, const Component& component) {
    return component.print(stream);
}

void Component::update() {
    if (this->parent == nullptr) {
        UI::clear();

        cout << *this;
    }
    else {
        this->parent->update();
    }
}

/**
 * Grid
 */
UI::Grid::Grid() : Grid({}, {}) {};

UI::Grid::~Grid() {
    while (!this->components.empty()) {
        vector<Component*> row = this->components.back();

        while (!row.empty()) {
            auto component = row.back();

            delete component;

            row.pop_back();
        }

        this->components.pop_back();
    }
}

UI::Grid::Grid(Component* component, Style style) : Grid({ {component} }, style) {};

UI::Grid::Grid(vector<vector<Component*>> components, Style style) : components(components), Component(style) {
    for (auto row : this->components) {
        for (auto component : row) {
            component->parent = this;
        }
    }
};

UI::Grid::Grid(const Grid& grid) {
    vector<vector<Component*>> componentsClone;

    for (auto row : grid.components) {
        vector<Component*> cloneRow;

        for (auto component : row) {
            cloneRow.push_back(component->clone());
        }

        componentsClone.push_back(cloneRow);
    }

    Grid(componentsClone, grid.style);
}

ostream& operator<<(ostream& stream, const Grid& grid) {
    if (grid.components.empty()) return stream;

    vector<vector<string>> data;

    for (auto row : grid.components) {
        vector<string> dataRow;

        for (auto component : row) {
            stringstream ss;
            ss << *component;
            dataRow.push_back(ss.str());
        }

        data.push_back(dataRow);
    }

    int padding = grid.style.padding;
    bool bordered = grid.style.border;

    int rowSize = data.size();
    int columnSize = data[0].size();
    vector<int> rowHeights(rowSize);
    vector<int> columnWidths(columnSize);

    vector<vector<vector<string>>> splitData(rowSize, vector<vector<string>>(columnSize, vector<string>()));

    for (auto [row, i] : enumerate(data)) {
        for (auto [e, j] : enumerate(row)) {
            vector<string> eSplit = split(e, "\n");

            for (auto string : eSplit) {
                splitData[i][j].push_back(string);
            }

            rowHeights[i] = max(rowHeights[i], (int)eSplit.size());

            columnWidths[j] = max(
                columnWidths[j],
                (int)max_element(
                    eSplit.begin(),
                    eSplit.end(),
                    [](string l, string r) {
                        return l.size() < r.size();
                    }
            )[0].size());
        }
    }

    // Structured binding, will compile with C++17. Make sure to change in settings if warning.
    for (auto [row, height] : zip2(splitData, rowHeights)) {
        if (bordered) {
            for (auto width : columnWidths) {
                stream << "+" << string(width + padding * 2, '-');
            }

            stream << "+" << endl;
        }

        for (auto _ : xrange(0, padding)) {
            for (auto width : columnWidths) {
                stream << (bordered ? "|" : "") << string(width + padding * 2, ' ');
            }

            stream << (bordered ? "|" : "") << endl;
        }

        for (auto i : xrange(0, height)) {
            for (auto [e, width] : zip2(row, columnWidths)) {
                stream << (bordered ? "|" : "") << string(padding, ' ');

                switch (grid.style.align) {
                case CENTER:
                    stream << string_center(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                case RIGHT:
                    stream << string_right(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                default:
                case LEFT:
                    stream << string_left(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                }

                stream << string(padding, ' ');
            }

            stream << (bordered ? "|" : "") << endl;
        }

        for (auto _ : xrange(0, padding)) {
            for (auto width : columnWidths) {
                stream << (bordered ? "|" : "") << string(width + padding * 2, ' ');
            }

            stream << (bordered ? "|" : "") << endl;
        }
    }

    if (bordered) {
        for (auto width : columnWidths) {
            stream << "+" << string(width + padding * 2, bordered ? '-' : ' ');
        }

        stream << "+" << endl;
    }

    return stream;
}

/**
 * List
 */
UI::List::List() : List(vector<string>{}, {}) {};

UI::List::~List() {
    cout << "List Destroy" << endl;

    while (!this->components.empty()) delete this->components.back();
};

UI::List::List(vector<Component*> components, Style style) : components(components), Component(style) {
    for (auto component : this->components) {
        component->parent = this;
    }
};

UI::List::List(vector<string> strings, Style style) : List(toText(strings), style) {};

UI::List::List(const List& list) {
    vector<Component*> componentsClone;

    for (auto component : list.components) {
        componentsClone.push_back(component->clone());
    }

    List(componentsClone, list.style);
}

ostream& operator<<(ostream& stream, const List& list) {
    stringstream ss;

    int c = 1;

    for (auto component : list.components) {
        if (list.style.enumerate) {
            ss << c++ << ". " << *component << endl;
        }
        else {
            ss << "- " << *component << endl;
        }
    }

    stream << Grid(new Text(ss.str()), list.style);

    return stream;
}

/**
 * Text
 */
UI::Text::Text() : Text("", {}) {};

UI::Text::~Text() {};

UI::Text::Text(string text, const Style& style) : text(text), Component(style) {};

UI::Text::Text(const Text& text) : Text(text.text, text.style) {};

ostream& operator<<(ostream& stream, const Text& text) {
    stream << text.text;

    return stream;
}

/**
 * Banner
 */
UI::Banner::~Banner() {};

UI::Banner::Banner(Style style) : Component(style) {};

UI::Banner::Banner(const Banner& banner) : Banner(banner.style) {};

ostream& operator<<(ostream& stream, const Banner& banner) {
    stream << " __    __   ____  ____   _____   ___   ____     ___ " << endl;
    stream << "|  |__|  | /    ||    \\ |     | /   \\ |    \\   /  _]" << endl;
    stream << "|  |  |  ||  o  ||  D  )|__/  ||     ||  _  | /  [_ " << endl;
    stream << "|  |  |  ||     ||    / |   __||  O  ||  |  ||    _]" << endl;
    stream << "|  `  '  ||  _  ||    \\ |  /  ||     ||  |  ||   [_ " << endl;
    stream << " \\      / |  |  ||  .  \\|     ||     ||  |  ||     |" << endl;
    stream << "  \\_/\\_/  |__|__||__|\\_||_____| \\___/ |__|__||_____|" << endl;

    return stream;
}

/**
 * Viewport
 */
UI::Viewport::Viewport() : Viewport(0, {}) {};

UI::Viewport::~Viewport() {
    if (this->component) {
        delete this->component;
    }
};

UI::Viewport::Viewport(Component* component, Style style) : component(component), Component(style) {
    this->component->parent = this;
};

UI::Viewport::Viewport(const Viewport& viewport) : Viewport(viewport.component->clone(), viewport.style) {};

ostream& operator<<(ostream& stream, const UI::Viewport& viewport) {
    stringstream ss;

    ss << *viewport.component;

    vector<string> lines = split(ss.str(), "\n");

    for (int i = viewport.style.y; i < viewport.style.y + viewport.style.height; i++) {
        if (i >= lines.size() || i < 0) {
            stream << string(viewport.style.width, ' ') << endl;
        }
        else {
            string line = lines[i];

            string frontPad = string(min(max(-viewport.style.x, 0), viewport.style.width), ' ');

            line = line.substr(
                min(
                    max(viewport.style.x, 0),
                    (int)line.size()
                ),
                max(viewport.style.width - max(-viewport.style.x, 0), 0)
            );

            string backPad = string(viewport.style.width - (int)frontPad.size() - (int)line.size(), ' ');

            stream
                << frontPad
                << line
                << backPad
                << endl;
        }
    }

    return stream;
}

/**
 * Methods
 */
int UI::ask(const string& prompt, vector<string> options) {
    UI::clear();

    cout << Grid({
        {new Text(prompt)},
        {new List(options, {.padding = 0, .border = false, .enumerate = true })}
        });

    int res = UI::validate(1, options.size());

    if (res >= 0) return res;

    return UI::ask(prompt, options);
}

int UI::ask(const Component& component, vector<string> options) {
    UI::clear();

    stringstream ss;

    ss << component;

    return UI::ask(ss.str(), options);
}

int UI::validate(int min, int max) {
    int option = 0;

    cout << endl << "> ";

    if (cin >> option) {
        if (option >= min && option <= max) {
            return option;
        }
    }

    UI::clear();

    cout << Grid(new Text("Please enter a valid option."), {});

    cout << endl;

    return -1;
}

int UI::range(string prompt, int min, int max) {
    UI::clear();

    stringstream ss;

    ss << "(" << min << "," << max << ")";

    cout << Grid({
        {new Text(prompt)},
        {new Text(ss.str())}
        });

    int res = UI::validate(min, max);

    if (res >= 0) return res;

    return UI::range(prompt, min, max);
}

void UI::clear() {
    cout << "\033[2J";
}

vector<Component*> UI::toText(vector<string> strings) {
    vector<Component*> texts;

    for (auto string : strings) {
        texts.push_back(new Text(string));
    }

    return texts;
}