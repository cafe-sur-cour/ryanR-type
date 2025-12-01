/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include <chrono>

#include "../common/Error/IError.hpp"

#include "Core.hpp"
#include "Utils.hpp"

int main(int ac, char **av) {
    Core core;
    Utils utils;

    try {
        utils.parseCli(ac, av, core.getNetwork());
        core.getNetwork()->init();
        core.run();
    } catch (const err::IError &e) {
        std::cerr << e.what() << std::endl;
        exit(84);
    } catch (const std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        exit(84);
    } catch (...) {
        std::cerr << "Unknown error occurred in client." << std::endl;
        exit(84);
    }
    return 0;
}
