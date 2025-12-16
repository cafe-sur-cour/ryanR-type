/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Server Config
*/

#include <cstdint>
#include <string>
#include "ServerConfig.hpp"
#include "../common/constants.hpp"

rserv::ServerConfig::ServerConfig() :
    _state(-1), _port(constants::DEFAULT_SERVER_PORT),
    _ip(constants::DEFAULT_SERVER_IP), _isDebug(false) {
}

rserv::ServerConfig::~ServerConfig() {
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

std::string rserv::ServerConfig::getIp() const {
    return this->_ip;
}

void rserv::ServerConfig::setIp(std::string ip) {
    this->_ip = ip;
}

void rserv::ServerConfig::setIsDebug(bool isDebug) {
    this->_isDebug = isDebug;
}

bool rserv::ServerConfig::getIsDebug() const {
    return this->_isDebug;
}
