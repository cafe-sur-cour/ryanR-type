/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config
*/

#include "ServerConfig.hpp"

/* Constructor and Destructor */

ServerConfig::ServerConfig(unsigned int port) :
    _state(-1), _fd(-1), _port(port) {
}

ServerConfig::~ServerConfig() {
}

/* Getters */

int ServerConfig::getState() const {
    return this->_state;
}

int ServerConfig::getFd() const {
    return this->_fd;
}

unsigned int ServerConfig::getPort() const {
    return this->_port;
}

/* Setters */

void ServerConfig::setState(int state) {
    this->_state = state;
}

void ServerConfig::setFd(int fd) {
    this->_fd = fd;
}

void ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}
