/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/

#include <iostream>
#include <memory>

#include "Core.hpp"
#include "Utils.hpp"

#include "../common/Error/IError.hpp"


int main(int ac, char **av) {
    Utils utils;
    Core core;

    utils.parsCli(ac, av, core.getConfig());
    try {
        core.init();
        core.loop();
    } catch (const err::IError &e) {
        std::cerr << e.getDetails() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "Unknown error occurred in server." << std::endl;
        return 84;
    }
    return 0;
}
