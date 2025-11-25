/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/
#include <iostream>

#include "Utils.hpp"

Utils::Utils()
{
}

Utils::~Utils()
{
}

void Utils::parsCli(int ac, char **av, std::shared_ptr<rserv::ServerConfig> config)
{
    int port = 0;
    int nbClients = 1;
    int ip = 0;

    for (int i = 1; i < ac; i++) {
        if (std::string(av[i]) == "-p" && i + 1 < ac) {
            port = std::stoi(av[i + 1]);
            config->setPort(static_cast<unsigned int>(port));
        }
        if (std::string(av[i]) == "-n" && i + 1 < ac) {
            nbClients = std::stoi(av[i + 1]);
        }
        if (std::string(av[i]) == "-i" && i + 1 < ac) {
            ip = std::stoi(av[i + 1]);
            config->setIp(static_cast<uint32_t>(ip));
        }
        if (std::string(av[i]) == "-h") {
            this->helper();
            exit(0);
        }
    }
    if (ac <= 4)  {
        this->helper();
        exit(84);
    }
    config->setFd(-1);
    if (nbClients < 1 || nbClients > 4) {
        helper();
        exit(84);
    }
    config->setNbClients(nbClients);
}

void Utils::helper()
{
    std::cout << "Usage: ./r-type_server [options]\n"
        << "Options:\n"
        << "\r-p <port>        Specify the port number\n"
        << "\r-i <ip_address>  Specify the IP address to bind to\n"
        << "\r-n <nb_clients>  Specify the maximum number of clients (Default: 1, Max: 4)\n"
        << "\r-h               Display this help message\n"
        << "Example:\n"
        << "  ./r-type_server -p 8080 -i 127.0.0.1 -n 4\n";
}
