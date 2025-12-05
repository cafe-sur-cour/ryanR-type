/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** debug
*/

#ifndef DEBUG_HPP_
#define DEBUG_HPP_

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #ifdef ERROR
        #undef ERROR
    #endif
    #ifdef INFO
        #undef INFO
    #endif
    #ifdef WARNING
        #undef WARNING
    #endif
#endif


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
