/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/

#include <iostream>
#include <memory>
#include <string>

#include "Utils.hpp"
#include "../common/constants.hpp"

Utils::Utils() {
}

Utils::~Utils() {
}

void Utils::helper() {
    std::cout << "Usage: ./r-type_client [options]\n"
        << "Options:\n"
        << "\r-p <port>        Specify the port of the server (default: 4242)\n"
        << "\r-i <ip_address>  "
        << "Specify the IP address of the server (default: 127.0.0.1)\n"
        << "\r-n <name>        Optional specify the name of the client\n"
        << "\r-h               Display this help message\n"
        << "Example:\n"
        << "\r./r-type_client -p 8080 -i 127.0.0.1\n"
        << "\r./r-type_client  # Uses default values\n";
}

void Utils::parseCli(int ac, char **av, std::shared_ptr<ClientNetwork>
    clientNetwork) {
    int port = constants::DEFAULT_SERVER_PORT;
    std::string ip = constants::DEFAULT_SERVER_IP;

    for (int i = 1; i < ac; i++) {
        std::string arg = av[i];
        if (arg == "-p" && i + 1 < ac) {
            port = std::stoi(av[i + 1]);
            i++;
        } else if (arg == "-i" && i + 1 < ac) {
            ip = av[i + 1];
            i++;
        } else if (arg == "-h") {
            this->helper();
            exit(0);
        } else if (arg == "-n" && i + 1 < ac) {
            clientNetwork->setName(av[i + 1]);
            i++;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            this->helper();
            exit(1);
        }
    }
    clientNetwork->setPort(port);
    clientNetwork->setIp(ip);
}
