/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** main.cpp
*/

#include <iostream>
#include <memory>
#include "Core.hpp"
#include "ClientNetwork.hpp"
#include "Utils.hpp"


int main(int ac, char **av) {
    Utils utils;
    std::shared_ptr<ClientNetwork> clientNetwork = std::make_shared<ClientNetwork>();

    utils.parseCli(ac, av, clientNetwork);
    Core core(clientNetwork);
    core.gameLoop();
    return 0;
}
