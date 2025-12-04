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

enum debugType {
    NETWORK = 0,
    ECS = 1,
    CORE = 2
};

enum debugLevel {
    INFO = 0,
    WARNING = 1,
    ERROR = 2
};

class Debug {
    public:
        ~Debug() = default;
        static void printDebug(const bool isDebug, const std::string &message, debugType type, debugLevel level);
};

} // namespace debug

#endif /* !DEBUG_HPP_ */
