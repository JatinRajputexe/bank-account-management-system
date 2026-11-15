#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// Small helper functions shared by all classes (formatting + safe console input).
namespace util {

// Fixed-precision currency, e.g. "Rs. 5000.00"
inline std::string money(double value) {
    std::ostringstream out;
    out << "Rs. " << std::fixed << std::setprecision(2) << value;
    return out.str();
}

// Rounds to 2 decimals so repeated additions never drift (0.1 + 0.2 problem).
inline double round2(double value) {
    return std::round(value * 100.0) / 100.0;
}

inline std::string trim(const std::string& text) {
    std::size_t start = text.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    std::size_t end = text.find_last_not_of(" \t\r\n");
    return text.substr(start, end - start + 1);
}

// '|' separates fields in the data file, so it must never appear inside a value.
inline std::string sanitize(std::string text) {
    for (char& c : text) {
        if (c == '|' || c == '\n' || c == '\r') c = ' ';
    }
    return text;
}

inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) throw std::runtime_error("Input closed.");
    return trim(line);
}

// Keeps asking until the user types a valid whole number.
inline int readInt(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t pos = 0;
            int value = std::stoi(line, &pos);
            if (pos == line.size()) return value;
        } catch (const std::exception&) {
        }
        std::cout << "Invalid number. Please try again.\n";
    }
}

// Keeps asking until the user types a valid finite number.
inline double readDouble(const std::string& prompt) {
    while (true) {
        std::string line = readLine(prompt);
        try {
            std::size_t pos = 0;
            double value = std::stod(line, &pos);
            if (pos == line.size() && std::isfinite(value)) return value;
        } catch (const std::exception&) {
        }
        std::cout << "Invalid amount. Please enter a number.\n";
    }
}

}  // namespace util

#endif
