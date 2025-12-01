/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include <chrono>

#include "Core.hpp"
#include "Utils.hpp"

int main(int ac, char **av) {
    Core core;
    Utils utils;

    utils.parseCli(ac, av, core.getNetwork());
    core.getNetwork()->init();
    core.run();
    return 0;
}
