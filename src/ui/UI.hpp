#pragma once

#include <string>
#include <curses.h>
#include <iostream>
#include <vector>
#include <Utils.hpp>
#include <algorithm>
#include <UI.fwd.hpp>

using namespace std;

ostream& operator<<(ostream& stream, const UI::Component& component);
ostream& operator<<(ostream& stream, const UI::Grid& grid);
ostream& operator<<(ostream& stream, const UI::Frame& frame);
ostream& operator<<(ostream& stream, const UI::List& list);
ostream& operator<<(ostream& stream, const UI::Text& text);
ostream& operator<<(ostream& stream, const UI::Banner& banner);
ostream& operator<<(ostream& stream, const UI::Viewport& viewport);

namespace UI {
    struct Style {
        int padding = 1;
        bool border = true;
        bool enumerate = false;
        int width = 5;
        int height = 5;
        int x = 0;
        int y = 0;
    };

    class Component {
    public:
        struct Style style;

        Component(struct Style style);

        friend ostream& ::operator<<(ostream& stream, const Component& component);

        virtual ostream& print(ostream& stream) const { stream << "Component"; return stream; };
    };

    class Grid : public Component {
    private:
        vector<vector<Component*>> components;
    public:
        Grid(vector<vector<Component*>> components, Style style = {});

        friend ostream& ::operator<<(ostream& stream, const Grid& grid);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    class Frame : public Component {
    private:
        Component* component;
    public:
        Frame(Component* component, Style style = {});

        friend ostream& ::operator<<(ostream& stream, const Frame& frame);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    class List : public Component {
    private:
        vector<Component*> components;
    public:
        List(vector<Component*> components, Style style = { .padding = 0, .border = false });
        List(vector<string> strings, Style style = { .padding = 0, .border = false });

        friend ostream& ::operator<<(ostream& stream, const List& list);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    class Text : public Component {
    public:
        string text;

        Text(string text, Style style = {});

        friend ostream& ::operator<<(ostream& stream, const Text& text);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    class Banner : public Component {
    public:
        Banner(Style style = {});

        friend ostream& ::operator<<(ostream& stream, const Banner& banner);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    class Viewport : public Component {
    private:
        Component* component;
    public:
        Viewport(Component* component, Style style = {});

        friend ostream& ::operator<<(ostream& stream, const Viewport& viewport);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };
    };

    /**
    * Clears the terminal screen.
    */
    void clear();

    /**
     * Asks the user a prompt until a valid option is selected.
     *
     * @param prompt Prompt to ask.
     * @param options Vector of answers for prompt.
     * @return Index of answerostream&
     */
    int ask(const string& prompt, vector<string> options);

    /**
     * Asks a user for a number in range.
     *
     * @param prompt Prompt to ask.
     * @param min Min value of prompt.
     * @param max Max value of prompt.
     * @return Value selected in range.
     */
    int range(string prompt, int min, int max);

    /**
     * Converts strings to Text component.
     * @param strings Strings to convert.
     * @return List of Text object.
     */
    vector<Component*> to_text(vector<string> strings);

    /**
     * Prompt for user data in range.
     */
    static int validate(int min, int max);
}