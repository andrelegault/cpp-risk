#pragma once

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
}