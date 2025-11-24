/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config
*/

#include "ServerConfig.hpp"

rserv::ServerConfig::ServerConfig(unsigned int port) :
    _state(-1), _fd(-1), _port(port) {
}

rserv::ServerConfig::~ServerConfig() {
}

int rserv::ServerConfig::getState() const {
    return this->_state;
}

int rserv::ServerConfig::getFd() const {
    return this->_fd;
}

unsigned int rserv::ServerConfig::getPort() const {
    return this->_port;
}

void rserv::ServerConfig::setState(int state) {
    this->_state = state;
}

void rserv::ServerConfig::setFd(int fd) {
    this->_fd = fd;
}

void rserv::ServerConfig::setPort(unsigned int port) {
    this->_port = port;
}
