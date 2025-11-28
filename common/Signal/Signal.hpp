/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Signal
*/

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

#ifdef _WIN32
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00
    #endif

    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif
#endif

#include <csignal>

class Signal {
    public:
        Signal();
        ~Signal();

        static volatile sig_atomic_t stopFlag;
        static void signalHandler(int signum);
        static void setupSignalHandlers();

    protected:
    private:
};

#endif /* !SIGNAL_HPP_ */
