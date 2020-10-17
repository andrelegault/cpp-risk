#include <UI.hpp>

using namespace UI;

/**
 * Style
 */
struct UI::Style Style;

/**
 * Component
 */
UI::Component::Component(Style style) : style(style) {};

ostream& operator<<(ostream& stream, const Component& component) {
    return component.print(stream);
}

/**
 * Grid
 */
UI::Grid::Grid(vector<vector<Component*>> components, Style style) : components(components), UI::Component(style) {};

ostream& operator<<(ostream& stream, const Grid& grid) {
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

                stream << string_center(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);

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
 * Frame
 */
UI::Frame::Frame(Component* component, Style style) : component(component), Component(style) {};

ostream& operator<<(ostream& stream, const Frame& frame) {
    stream << Grid({ {frame.component} }, frame.style);

    return stream;
}

/**
 * List
 */
UI::List::List(vector<Component*> components, Style style) : components(components), Component(style) {};
UI::List::List(vector<string> strings, Style style) : components(to_text(strings)), Component(style) {};

ostream& operator<<(ostream& stream, const List& list) {
    int c = 1;

    for (auto component : list.components) {
        if (list.style.enumerate) {
            stream << c++ << ". " << *component << endl;
        }
        else {
            stream << "- " << *component << endl;
        }
    }

    return stream;
}

/**
 * Text
 */
UI::Text::Text(string text, Style style) : text(text), Component(style) {};

ostream& operator<<(ostream& stream, const Text& text) {
    stream << text.text;

    return stream;
}

/**
 * Banner
 */
UI::Banner::Banner(Style style) : Component(style) {};

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
UI::Viewport::Viewport(Component* component, Style style) : component(component), Component(style) {};

ostream& operator<<(ostream& stream, const UI::Viewport& viewport) {
    stringstream ss;

    ss << *viewport.component;

    vector<string> lines = split(ss.str(), "\n");

    for(int i = viewport.style.y; i < viewport.style.y + viewport.style.height; i++) {
        if(i >= lines.size() || i < 0) {
            stream << string(viewport.style.width, ' ') << endl;
        } else {
            string line = lines[i].substr(viewport.style.x, viewport.style.width);

            stream << line << string(viewport.style.width - line.size(), ' ') << endl;
        }
    }

    return stream;
}

/**
 * Methods
 */
int UI::ask(const string& prompt, vector<string> options) {
    cout << Grid({
        {new Text(prompt)},
        {new List(options, { .padding = 0, .border=false, .enumerate = true })}
        });

    int res = UI::validate(1, options.size());

    if (res >= 0) return res;

    return UI::ask(prompt, options);
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

    cout << Frame(new Text("Please enter a valid option."));

    cout << endl;

    return -1;
}

int UI::range(string prompt, int min, int max) {
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

vector<Component*> UI::to_text(vector<string> strings) {
    vector<Component*> texts;

    for (auto string : strings) {
        texts.push_back(new Text(string));
    }

    return texts;
}