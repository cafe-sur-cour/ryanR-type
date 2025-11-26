/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Signal
*/

#include "Signal.hpp"

volatile sig_atomic_t Signal::stopFlag = 0;

Signal::Signal() {
}

Signal::~Signal() {
}

void Signal::signalHandler(int signum) {
    (void)signum;
    stopFlag = 1;
}

void Signal::setupSignalHandlers() {
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGTSTP, signalHandler);
    signal(SIGTERM, signalHandler);
}
