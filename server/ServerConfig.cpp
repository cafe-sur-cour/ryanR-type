/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config
*/

#include "ServerConfig.hpp"

rserv::ServerConfig::ServerConfig() :
    _state(-1), _fd(-1), _port(0), _nbClients(0) {
}

rserv::ServerConfig::~ServerConfig() {
}


int rserv::ServerConfig::getFd() const {
    return this->_fd;
}

void rserv::ServerConfig::setFd(int fd) {
    this->_fd = fd;
}

void rserv::ServerConfig::setState(int state) {
    this->_state = state;
}

int rserv::ServerConfig::getState() const {
    return this->_state;
}


void rserv::ServerConfig::setPort(uint16_t port) {
    this->_port = port;
}

uint16_t rserv::ServerConfig::getPort() const {
    return this->_port;
}

void rserv::ServerConfig::setNbClients(int nbClients) {
    this->_nbClients = nbClients;
}

int rserv::ServerConfig::getNbClients() const {
    return this->_nbClients;
}

std::string rserv::ServerConfig::getIp() const {
    return this->_ip;
}

void rserv::ServerConfig::setIp(std::string ip) {
    this->_ip = ip;
}
