/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/

#include <iostream>
#include <memory>

#include "Server.hpp"
#include "Utils.hpp"
#include "Constants.hpp"

#include "../common/Error/IError.hpp"


int main(int ac, char **av) {
    Utils utils;
    rserv::Server core;

    utils.parsCli(ac, av, core.getConfig());
    std::cout << "[Server] RTYPE SERVER" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "Port: " << core.getConfig()->getPort() << std::endl;
    std::cout << "IP: " << core.getConfig()->getIp() << std::endl;
    std::cout << "Max Clients: " << constants::MAX_CLIENT;
    std::cout << std::endl << "------------------------" << std::endl;
    try {
        core.init();
        core.start();
    } catch (const err::IError &e) {
        std::cerr << e.getDetails() << std::endl;
        return (84);
    } catch (const std::runtime_error &e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        return (84);
    } catch (...) {
        std::cerr << "Unknown error occurred in server." << std::endl;
        return (84);
    }
    return 0;
}
