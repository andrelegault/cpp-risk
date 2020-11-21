#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Utils.hpp"
#include "Style.hpp"
#include "Component.hpp"
#include "Banner.hpp"
#include "Grid.hpp"
#include "List.hpp"
#include "Text.hpp"
#include "Viewport.hpp"

namespace UI {
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
    int ask(const std::string& prompt, std::vector<std::string> options);

    /**
     * Overload of string ask. Uses component for input.
     * 
     * @param component Component to use to prompt.
     * @param options Vector of answers for prompt.
     * @return Index of answer.
     */
    int ask(const UI::Component& component, std::vector<std::string> options);

    /**
     * Asks a user for a number in range.
     *
     * @param prompt Prompt to ask.
     * @param min Min value of prompt.
     * @param max Max value of prompt.
     * @return Value selected in range.
     */
    int range(std::string prompt, int min, int max);

    /**
     * Converts strings to Text component.
     * @param strings Strings to convert.
     * @return List of Text object.
     */
    std::vector<UI::Component*> toText(std::vector<std::string> strings);

    /**
     * Prompt for user data in range.
     * 
     * @param min Min value of data (including).
     * @param max Max vaue of data (including).
     * @return Value between min and max (including).
     */
    int validate(int min, int max);

    /**
     * Waits for user to key press.
     */
    void waitForInput();
}
