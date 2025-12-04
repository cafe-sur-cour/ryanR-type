/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** debug
*/

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#include <string>

namespace debug {

enum class debugType {
    NETWORK,
    ECS,
    CORE
};

enum class debugLevel {
    INFO,
    WARNING,
    ERROR
};

class Debug {
    public:
        ~Debug() = default;
        static void printDebug(const bool isDebug, const std::string &message, const debugType type, const debugLevel level);
};

}

#endif /* !DEBUG_HPP_ */
