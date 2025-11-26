/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/

#include <iostream>

#include "Utils.hpp"

Utils::Utils() {
}

Utils::~Utils() {
}

void Utils::helper() {
    std::cout << "Usage: ./r-type_client [options]\n"
        << "Options:\n"
        << "\r-p <port>        Specify the port of the server\n"
        << "\r-i <ip_address>  Specify the IP address of the server\n"
        << "\r-n <name>        Optional specify the name of the client\n"
        << "\r-h               Display this help message\n"
        << "Example:\n"
        << "\r./r-type_client -p 8080 -i 127.0.0.1\n";
}

void Utils::parseCli(int ac, char **av, std::shared_ptr<ClientNetwork> clientNetwork) {
    int port = 0;
    int ip = 0;

    for (int i = 1; i < ac; i++) {
        std::string arg = av[i];
        if (arg == "-p" && i + 1 < ac) {
            port = std::stoi(av[i + 1]);
            i++;
        } else if (arg == "-i" && i + 1 < ac) {
            ip = std::stoi(av[i + 1]);
            i++;
        } else if (arg == "-h") {
            this->helper();
            exit(0);
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            this->helper();
            exit(1);
        }
    }
    if (ac < 4) {
        this->helper();
        exit(84);
    }
    if (port != 0) {
        clientNetwork->setPort(port);
    }
    if (ip != 0) {
        clientNetwork->setIp(ip);
    }
}
