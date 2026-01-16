/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/
#include <iostream>
#include <string>
#include <memory>
#include <random>
#include <algorithm>

#include "Utils.hpp"
#include "constants.hpp"

Utils::Utils() {
}

Utils::~Utils() {
}

void Utils::parsCli(int ac, char **av,
    std::shared_ptr<rserv::ServerConfig> config) {
    int port = constants::DEFAULT_SERVER_PORT;
    std::string ip = constants::DEFAULT_SERVER_IP;
    bool isDebug = false;

    for (int i = 1; i < ac; i++) {
        if (std::string(av[i]) == "-p" && i + 1 < ac) {
            i++;
            port = std::stoi(av[i]);
            if (port < 1024 || port > 65535) {
                std::cerr << "[SERVER] Error: Port must be between 1024 and 65535"
                    << std::endl;
                exit(84);
            }
            try {
                config->setPort(static_cast<uint16_t>(port));
            } catch (const std::invalid_argument &e) {
                std::cerr << "[SERVER] Error: " << e.what() << std::endl;
                exit(84);
            }
            continue;
        }
        if (std::string(av[i]) == "-i" && i + 1 < ac) {
            i++;
            ip = av[i];
            config->setIp(ip);
            continue;
        }
        if (std::string(av[i]) == "-d") {
            isDebug = true;
            config->setIsDebug(isDebug);
            continue;
        }
        if (std::string(av[i]) == "-tps" && i + 1 < ac) {
            i++;
            int64_t tps = std::stoi(av[i]);
            if (tps <= 0) {
                std::cerr << "[SERVER] Error: TPS must be greater than 0" << std::endl;
                exit(84);
            }
            config->setTps(tps);
            continue;
        }
        if (std::string(av[i]) == "-h") {
            this->helper();
            exit(0);
        }
        std::cerr << "[SERVER] Error: Unknown argument " << av[i] << std::endl;
        exit(84);
    }
}


void Utils::helper() {
    std::cout << "Usage: ./r-type_server [options]\n"
        << "Options:\n"
        << "\r-p <port>        Specify the port number (default: "
            << constants::DEFAULT_SERVER_PORT << ") (5173 is reserved for HTTP server)\n"
        << "\r-i <ip_address>  Specify the IP address to bind to (default: "
            << constants::DEFAULT_SERVER_IP << ")\n"
        << "\r-tps <tps>       Specify the TPS (ticks per second) (default: "
            << constants::TPS << ")\n"
        << "\r-d               Enable debug mode\n"
        << "\r-h               Display this help message\n"
        << "Example:\n"
        << "  ./r-type_server -p 4243 -i 127.0.0.1\n";
}

std::string Utils::createAlphaNumericCode() {
    static const std::string charset = "ABCDEFGHJKLMNPQRSTUVWXYZ23456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<std::size_t> dis(0, charset.size() - 1);

    std::string code;
    code.reserve(8);
    for (int i = 0; i < 8; ++i) {
        code += charset[dis(gen)];
    }
    return code;
}
