#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <Utils.hpp>
#include <algorithm>
#include "UI.fwd.hpp"

using namespace std;

ostream& operator<<(ostream& stream, const UI::Component& component);
ostream& operator<<(ostream& stream, const UI::Grid& grid);
ostream& operator<<(ostream& stream, const UI::Frame& frame);
ostream& operator<<(ostream& stream, const UI::List& list);
ostream& operator<<(ostream& stream, const UI::Text& text);
ostream& operator<<(ostream& stream, const UI::Banner& banner);
ostream& operator<<(ostream& stream, const UI::Viewport& viewport);

/**
 * Namespace for UI components.
 */
namespace UI {
    /**
     * Alignment enum for Style.
     */
    enum Alignment {
        CENTER,
        LEFT,
        RIGHT
    };

    /**
     * Generic style for UI Components (inspired by CSS rectangle model).
     */
    struct Style {
        /**
         * Border padding.
         */
        int padding = 1;
        /**
         * Is border visible?
         */
        bool border = true;
        /**
         * Does the component use enumeration?
         */
        bool enumerate = false;
        /**
         * Width of component.
         */
        int width = 5;
        /**
         * Height of component.
         */
        int height = 5;
        /**
         * X position of component.
         */
        int x = 0;
        /**
         * Y position of component.
         */
        int y = 0;
        /**
         * Alignment of component.
         */
        Alignment align = LEFT;
    };

    /**
     * Generic UI Component.
     */
    class Component {
    public:
        /**
         * The component style.
         */
        Style style;

        Component* parent = NULL;

        Component();

        ~Component();

        Component(Style style);

        Component(const Component& component);

        friend ostream& ::operator<<(ostream& stream, const Component& component);

        /**
         * Used for virtual operator<<.
         */
        virtual ostream& print(ostream& stream) const = 0;

        /**
         * Used for virtual clone constructor.
         */
        virtual Component* clone() const = 0;

        /**
         * Updates the component (and it's parent hierachy).
         */
        void update();
    };

    /**
     * A UI grid (alot like HTML table).
     */
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

    /**
     * A UI list (alot like HTML ul/l).
     */
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

    /**
     * A UI text (alot like HTML p, span, h1.., etc)
     */
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

    /**
     * A UI component to display the game banner (could be generalized to create a large ASCII font).
     */
    class Banner : public Component {
    public:
        ~Banner();

        Banner(Style style = {});

        Banner(const Banner& banner);

        friend ostream& ::operator<<(ostream& stream, const Banner& banner);

        virtual ostream& print(ostream& stream) const override { stream << *this; return stream; };

        virtual Component* clone() const override { return new Banner(*this); };
    };

    /**
     * A UI viewing component (alot like HTML div).
     */
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
     * @return Index of answer.
     */
    int ask(const string& prompt, vector<string> options);

    /**
     * Overload of string ask. Uses component for input.
     * 
     * @param component Component to use to prompt.
     * @param options Vector of answers for prompt.
     * @return Index of answer.
     */
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
    vector<Component*> toText(vector<string> strings);

    /**
     * Prompt for user data in range.
     * 
     * @param min Min value of data (including).
     * @param max Max vaue of data (including).
     * @return Value between min and max (including).
     */
    int validate(int min, int max);
}