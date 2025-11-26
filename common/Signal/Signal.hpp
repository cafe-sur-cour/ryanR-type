/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Signal
*/

#ifndef SIGNAL_HPP_
#define SIGNAL_HPP_

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
