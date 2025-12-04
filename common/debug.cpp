/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** debug
*/

#include <iostream>
#include <vector>
#include <string>

#include "debug.hpp"


void debug::Debug::printDebug(const bool isDebug, const std::string &message,
    const debugType type, const debugLevel level) {
    const std::vector<std::string> color = {"\033[0;96m", "\033[0;93m", "\033[0;91m" };
    const std::vector<std::string> typeStr = {"[NETWORK]", "[ECS]", "[CORE]" };

    if (!isDebug) {
        return;
    }
    for (size_t i = 0; i < typeStr.size(); i++) {
        if (static_cast<size_t>(type) == i) {
            for (size_t j = 0; j < color.size(); j++) {
                if (static_cast<size_t>(level) == j) {
                    std::cout << color[j] << typeStr[i] << " "
                        << message << "\033[0m" << std::endl;
                    return;
                }
            }
        }
    }
}
