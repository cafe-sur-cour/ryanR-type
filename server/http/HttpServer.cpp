/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** HttpServer
*/

#include "HttpServer.hpp"
#include "../Constants.hpp"
#include <httplib.h>
#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

rserv::HttpServer::HttpServer(
    std::function<bool()> statusChecker,
    std::function<ServerInfo()> infoGetter
) :
    _running(false),
    _statusChecker(statusChecker),
    _infoGetter(infoGetter),
    _server(nullptr) {
    this->loadEnv();
}

rserv::HttpServer::~HttpServer() {
    this->stop();
}

void rserv::HttpServer::start() {
    if (_running.load()) {
        return;
    }
    _running.store(true);
    _httpThread = std::thread([this]() { this->httpLoop(); });
}

void rserv::HttpServer::stop() {
    _running.store(false);
    if (_server) {
        _server->stop();
    }
    if (_httpThread.joinable()) {
        _httpThread.join();
    }
}

void rserv::HttpServer::loadEnv() {
    std::ifstream envFile(constants::HTTP_ENV_FILE_PATH);
    if (!envFile.is_open()) {
        std::cerr <<
            "Warning: .env file not found in server/http/. Using default password."
            << std::endl;
        _password = constants::DEFAULT_HTTP_PASSWORD;
        return;
    }
    std::string line;
    while (std::getline(envFile, line)) {
        if (line.find("HTTP_PASSWORD=") == 0) {
            _password = line.substr(14);
            break;
        }
    }
    envFile.close();
    if (_password.empty()) {
        std::cerr <<
            "Warning: HTTP_PASSWORD not found in .env. Using default password."
            << std::endl;
        _password = constants::DEFAULT_HTTP_PASSWORD;
    }
}

bool rserv::HttpServer::checkAuth(const httplib::Request &req) {
    auto authHeader = req.get_header_value("Authorization");
    if (authHeader.empty() || authHeader.find("Bearer ") != 0) {
        return false;
    }
    std::string token = authHeader.substr(7);
    return token == _password;
}

void rserv::HttpServer::httpLoop() {
    _server = std::make_unique<httplib::Server>();

    _server->Get("/api/status", [this](const httplib::Request &req, httplib::Response &res) {
        this->statusEndpoint(req, res);
    });
    _server->Get("/api/info", [this](const httplib::Request &req, httplib::Response &res) {
        this->infoEndpoint(req, res);
    });
    _server->set_mount_point("/", "./frontend/dist");
    std::cout << "HTTP server starting on port " << constants::HTTP_SERVER_PORT << std::endl;
    _server->listen("0.0.0.0", constants::HTTP_SERVER_PORT);
}

void rserv::HttpServer::infoEndpoint(const httplib::Request &req, httplib::Response &res) {
    if (!this->checkAuth(req)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
    }

    try {
        ServerInfo info = _infoGetter();

        nlohmann::json jsonResponse = {
            {"connectedClients", info.connectedClients},
            {"uptime", info.uptime.count()},
            {"activeLobbies", info.activeLobbies},
            {"totalPlayers", info.totalPlayers},
            {"lobbyDetails", info.lobbyDetails},
            {"playerDetails", info.playerDetails}
        };

        res.set_content(jsonResponse.dump(), "application/json");
    } catch (const std::exception &e) {
        res.status = 500;
        res.set_content("Internal server error", "text/plain");
    }
}

void rserv::HttpServer::statusEndpoint(const httplib::Request &req, httplib::Response &res) {
    if (!this->checkAuth(req)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
    }
    if (_statusChecker()) {
        res.set_content("Online", "text/plain");
    } else {
        res.set_content("Offline", "text/plain");
    }
}
