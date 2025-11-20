/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server
*/

#include <string>
#include <memory>
#include <iostream>
#include <asio.hpp>
#include "Server.hpp"

/* Constructor and Destructor */

Server::Server(unsigned int port) {
    this->_config = std::make_shared<ServerConfig>(port);
}

Server::~Server() {
    if (this->getState() == 1)
        this->stop();
}

/* Lifecycle */

void Server::init() {
    this->setState(0);
    std::cout << "[Server] Initialization complete on port "
        << _config->getPort() << std::endl;
}

void Server::start() {
    if (this->getState() == 1) {
        std::cerr <<
            "[Server] Error: init() must be called before start()"
            << std::endl;
        return;
    }

    std::cout << "[Server] Starting UDP server..." << std::endl;
    asio::io_context io;
    asio::ip::udp::socket socket(io,
        asio::ip::udp::endpoint(
            asio::ip::udp::v4(),
            _config->getPort()));
    this->setFd(1);
    std::array<char, 1024> buffer; /* Buffer for data (waiting buffer lib) */
    asio::ip::udp::endpoint remote;

    std::cout << "[Server] Waiting for UDP messages..." << std::endl;
    this->setState(1);
    /* This loop must be changed */ while (this->getState() == 1) {
        std::size_t size = socket.receive_from(asio::buffer(buffer), remote);
        buffer[size] = '\0';
        std::cout << "[UDP] Received: " << buffer.data() << " from "
            << remote.address().to_string() << ":"
            << remote.port() << std::endl;
        std::string response = "OK";
        socket.send_to(asio::buffer(response), remote);
    }
}

void Server::stop() {
    if (this->getState() == -1) {
        std::cerr <<
        "[Server] Error: init() must be called before stop()"
        << std::endl;
        return;
    }
    if (this->getState() == 0) {
        std::cerr << "[Server] Error: Server is not running." << std::endl;
        return;
    }
    this->setState(0);
    std::cout << "[Server] Server stopped." << std::endl;
}

/* Operators */

Server::operator int() const noexcept {
    return this->getState();
}

/* Getters */

std::shared_ptr<ServerConfig> Server::getConfig() const {
    return this->_config;
}

int Server::getState() const {
    return this->_config->getState();
}

int Server::getFd() const {
    return this->_config->getFd();
}

unsigned int Server::getPort() const {
    return this->_config->getPort();
}

/* Setters */

void Server::setState(int state) {
    this->_config->setState(state);
}

void Server::setFd(int fd) {
    this->_config->setFd(fd);
}

void Server::setPort(unsigned int port) {
    this->_config->setPort(port);
}
