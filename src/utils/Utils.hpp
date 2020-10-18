#pragma once

#include <string>
#include <vector>
#include <regex>
#include <iostream>
#include <utility>

using namespace std;

/**
 * Trim front and end of string of various whitespace characters by regex
 * @param line input
 * @return trimmed string
 */
string trim(string& line);

/**
 * Remove the first word of a string based on a delimiter
 * @param line
 * @param delim
 * @return modified string
 */
string trim_first_word(const string& line, const string& delim = " \t");

/**
 * Split a string into an array of words based on a delimiter
 * @param line
 * @param delim
 * @return vector<string> words
 */
vector<string> split(const string& line, const string& delim = " ");

/**
 * Centers text with a certain width.
 * Taken from https://stackoverflow.com/a/18600103.
 * 
 * @param string String to center.
 * @param width Width to reach.
 * @return Centered string.
 */
string string_center(const string string, const int width);

string string_left(const string s, const int w);

string string_right(const string s, const int w);

/**
 * Python-like range.
 * @param min Included min value.
 * @param max Excluded max value.
 * @param step Increase step.
 */
template<typename T>
vector<T> xrange(T min = 0, T max = 0, T step = 1) {
    vector<T> output;

    for(T i = min; i < max; i += step) {
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
vector<pair<T, U>> zip2(vector<T> v1, vector<U> v2) {
    vector<pair<T, U>> output;

    int size = min(v1.size(), v2.size());

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
vector<pair<T, int>> enumerate(vector<T> v, int offset = 0) {
    vector<int> indices;

    for (int i = 0; i < v.size(); i++) indices.push_back(i + offset);

    return zip2(v, indices);
}