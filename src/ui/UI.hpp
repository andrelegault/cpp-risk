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
    enum Alignment {
        CENTER,
        LEFT,
        RIGHT
    };

    struct Style {
        int padding = 1;
        bool border = true;
        bool enumerate = false;
        int width = 5;
        int height = 5;
        int x = 0;
        int y = 0;
        Alignment align = LEFT;
    };

    class Component {
    public:
        Style style;

        Component* parent = nullptr;

        Component();

        ~Component();

        Component(Style style);

        Component(const Component& component);

        friend ostream& ::operator<<(ostream& stream, const Component& component);

        virtual ostream& print(ostream& stream) const { stream << "Component"; return stream; };

        virtual Component* clone() const { return new Component(*this); };

        void update();
    };

    class Grid : public Component {
    private:
        vector<vector<Component*>> components;
    public:
        Grid();

        ~Grid();

        Grid(Component* component, Style style = {});

        Grid(vector<vector<Component*>> components, Style style = {});

        Grid(const Grid& grid);

        friend ostream& ::operator<<(ostream& stream, const Grid& grid);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new Grid(*this); };
    };

    class List : public Component {
    private:
        vector<Component*> components;
    public:
        List();

        ~List();

        List(vector<Component*> components, Style style = { .padding = 0, .border = false });

        List(vector<string> strings, Style style = { .padding = 0, .border = false });

        List(const List& list);

        friend ostream& ::operator<<(ostream& stream, const List& list);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new List(*this); };
    };

    class Text : public Component {
    public:
        string text;

        Text();

        ~Text();

        Text(string text, const Style& style = {});

        Text(const Text& text);

        friend ostream& ::operator<<(ostream& stream, const Text& text);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new Text(*this); };
    };

    class Banner : public Component {
    public:
        ~Banner();

        Banner(Style style = {});

        Banner(const Banner& banner);

        friend ostream& ::operator<<(ostream& stream, const Banner& banner);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new Banner(*this); };
    };

    class Viewport : public Component {
    private:
        Component* component;
    public:
        Viewport();

        ~Viewport();

        Viewport(Component* component, Style style = {});

        Viewport(const Viewport& viewport);

        friend ostream& ::operator<<(ostream& stream, const Viewport& viewport);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new Viewport(*this); };
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

    int ask(const Component& component, vector<string> options);

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