/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server
*/

#include <memory>
#include "Server.hpp"

Server::Server(unsigned int port) {
    this->_config = std::make_shared<ServerConfig>(port);
}

Server::~Server() {
    if (this->getState() == 1)
        this->stop();
}

void Server::init() {
    this->getConfig()->setState(0);
}

void Server::start() {
    this->getConfig()->setState(1);
}

void Server::stop() {
    this->getConfig()->setState(0);
}

Server::operator int() const noexcept {
    return this->getState();
}

std::shared_ptr<ServerConfig> Server::getConfig() {
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
