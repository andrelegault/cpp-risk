#include "Grid.hpp"

UI::Grid::Grid() : Grid({}, {}) {};

UI::Grid::~Grid() {
    while (!this->components.empty()) {
        std::vector<UI::Component*> row = this->components.back();

        while (!row.empty()) {
            auto component = row.back();

            delete component;

            row.pop_back();
        }

        this->components.pop_back();
    }
}

UI::Grid::Grid(UI::Component* component, UI::Style style) : Grid({ {component} }, style) {};

UI::Grid::Grid(std::vector<std::vector<UI::Component*>> components, UI::Style style) : components(components), UI::Component(style) {
    for (auto row : this->components) {
        for (auto component : row) {
            component->parent = this;
        }
    }
};

UI::Grid::Grid(const UI::Grid& grid) {
    std::vector<std::vector<UI::Component*>> componentsClone;

    for (auto row : grid.components) {
        std::vector<UI::Component*> cloneRow;

        for (auto component : row) {
            cloneRow.push_back(component->clone());
        }

        componentsClone.push_back(cloneRow);
    }

    UI::Grid(componentsClone, grid.style);
}

std::ostream& operator<<(std::ostream& stream, const UI::Grid& grid) {
    if (grid.components.empty()) return stream;

    std::vector<std::vector<std::string>> data;

    for (auto row : grid.components) {
        std::vector<std::string> dataRow;

        for (auto component : row) {
            std::stringstream ss;
            ss << *component;
            dataRow.push_back(ss.str());
        }

        data.push_back(dataRow);
    }

    int padding = grid.style.padding;
    bool bordered = grid.style.border;

    int rowSize = data.size();
    int columnSize = data[0].size();
    std::vector<int> rowHeights(rowSize);
    std::vector<int> columnWidths(columnSize);

    std::vector<std::vector<std::vector<std::string>>> splitData(rowSize, std::vector<std::vector<std::string>>(columnSize, std::vector<std::string>()));

    for (auto [row, i] : Utils::enumerate(data)) {
        for (auto [e, j] : Utils::enumerate(row)) {
            std::vector<std::string> eSplit = Utils::split(e, "\n");

            for (auto string : eSplit) {
                splitData[i][j].push_back(string);
            }

            rowHeights[i] = std::max(rowHeights[i], (int)eSplit.size());

            columnWidths[j] = std::max(
                columnWidths[j],
                (int)max_element(
                    eSplit.begin(),
                    eSplit.end(),
                    [](std::string l, std::string r) {
                        return l.size() < r.size();
                    }
            )[0].size());
        }
    }

    // Structured binding, will compile with C++17. Make sure to change in settings if warning.
    for (auto [row, height] : Utils::zip2(splitData, rowHeights)) {
        if (bordered) {
            for (auto width : columnWidths) {
                stream << "+" << std::string(width + padding * 2, '-');
            }

            stream << "+" << std::endl;
        }

        for (auto _ : Utils::xrange(0, padding)) {
            for (auto width : columnWidths) {
                stream << (bordered ? "|" : "") << std::string(width + padding * 2, ' ');
            }

            stream << (bordered ? "|" : "") << std::endl;
        }

        for (auto i : Utils::xrange(0, height)) {
            for (auto [e, width] : Utils::zip2(row, columnWidths)) {
                stream << (bordered ? "|" : "") << std::string(padding, ' ');

                switch (grid.style.align) {
                case UI::Alignment::CENTER:
                    stream << Utils::string_center(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                case UI::Alignment::RIGHT:
                    stream << Utils::string_right(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                default:
                case UI::Alignment::LEFT:
                    stream << Utils::string_left(((i < e.size() && !e[i].empty()) ? e[i] : " "), width);
                    break;
                }

                stream << std::string(padding, ' ');
            }

            stream << (bordered ? "|" : "") << std::endl;
        }

        for (auto _ : Utils::xrange(0, padding)) {
            for (auto width : columnWidths) {
                stream << (bordered ? "|" : "") << std::string(width + padding * 2, ' ');
            }

            stream << (bordered ? "|" : "") << std::endl;
        }
    }

    if (bordered) {
        for (auto width : columnWidths) {
            stream << "+" << std::string(width + padding * 2, bordered ? '-' : ' ');
        }

        stream << "+" << std::endl;
    }

    return stream;
}