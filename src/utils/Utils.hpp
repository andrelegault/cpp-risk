#pragma once

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <utility>

namespace Utils {
    /**
     * Trim front and end of string of various whitespace characters by regex
     * @param line input
     * @return trimmed string
     */
    std::string trim(std::string& line);

    /**
     * Remove the first word of a string based on a delimiter
     * @param line
     * @param delim
     * @return modified string
     */
    std::string trim_first_word(const std::string& line, const std::string& delim = " \t");

    /**
     * Split a string into an array of words based on a delimiter
     * @param line
     * @param delim
     * @return vector<string> words
     */
    std::vector<std::string> split(const std::string& line, const std::string& delim = " ");

    /**
     * Centers text with a certain width.
     * Taken from https://stackoverflow.com/a/18600103.
     *
     * @param str String to center.
     * @param width Width to reach.
     * @return Centered string.
     */
    std::string string_center(const std::string str, const int width);

    /**
     * Pads end of string to fit certain length.
     *
     * @param str String to pad.
     * @param width Width.
     * @return Left string.
     */
    std::string string_left(const std::string str, const int width);

    /**
     * Pads front of string to fit certain length.
     *
     * @param str String to pad.
     * @param width Width.
     * @return Right string.
     */
    std::string string_right(const std::string str, const int width);

    /**
     * Python-like range.
     *
     * @param min Included min value.
     * @param max Excluded max value.
     * @param step Increase step.
     */
    template<typename T>
    std::vector<T> xrange(T min = 0, T max = 0, T step = 1) {
        std::vector<T> output;

        for (T i = min; i < max; i += step) {
            output.push_back(i);
        }

        return output;
    }

    /**
     * Creates an iterator (vector) between two vectors.
     * Yields till either list is empty.
     *
     * (Generic functions must be declared in header file...)
     * Inspired by Python's implementation
     *
     * @param v1
     * @param v2
     * @return Vector of pairs.
     */
    template<typename T, typename U>
    std::vector<std::pair<T, U>> zip2(std::vector<T> v1, std::vector<U> v2) {
        std::vector<std::pair<T, U>> output;

        int size = std::min(v1.size(), v2.size());

        for (int i = 0; i < size; i++) {
            output.push_back({ v1[i], v2[i] });
        }

        return output;
    }

    /**
     * Creates indicies pair for a vector.
     *
     * (Generic functions must be declared in header file...)
     * Inspired by Python's implementation
     *
     * @param v
     * @param offset Value to start at.
     * @return Values with indices.
     */
    template<typename T>
    std::vector<std::pair<T, int>> enumerate(std::vector<T> v, int offset = 0) {
        std::vector<int> indices;

        for (int i = 0; i < v.size(); i++) indices.push_back(i + offset);

        return zip2(v, indices);
    }

    /**
     * Generates a random integer within min and max (inclusive).
     * @param min Minimum number for the range.
     * @param max Maximum number for the range.
     * @return Random number within range.
     */
    int getRandom(int min, int max);
}