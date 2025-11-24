/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config
*/

#include "ServerConfig.hpp"

net::ServerConfig::ServerConfig(unsigned int port) :
    _state(-1), _fd(-1), _port(port) {
}

net::ServerConfig::~ServerConfig() {
}

int net::ServerConfig::getState() const {
    return this->_state;
}

int net::ServerConfig::getFd() const {
    return this->_fd;
}

unsigned int net::ServerConfig::getPort() const {
    return this->_port;
}

void net::ServerConfig::setState(int state) {
    this->_state = state;
}

void net::ServerConfig::setFd(int fd) {
    this->_fd = fd;
}

void net::ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}
