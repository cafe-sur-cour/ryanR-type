/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** HttpServer
*/

#include "HttpServer.hpp"
#include <httplib.h>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>
#include "../constants.hpp"

rserv::HttpServer::HttpServer(
    std::function<bool()> statusChecker,
    std::function<ServerInfo()> infoGetter,
    std::shared_ptr<ServerConfig> serverConfig,
    std::function<std::string(const std::string&)> commandExecutor
) :
    _running(false),
    _statusChecker(statusChecker),
    _infoGetter(infoGetter),
    _serverConfig(serverConfig),
    _commandExecutor(commandExecutor),
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
    _server->Get("/api/config", [this](const httplib::Request &req, httplib::Response &res) {
        this->configEndpoint(req, res);
    });
    _server->Get("/api/commands/suggestions", [this](
        const httplib::Request &req, httplib::Response &res) {
        this->commandsSuggestionsEndpoint(req, res);
    });
    _server->Post("/api/commands/execute", [this](
        const httplib::Request &req, httplib::Response &res) {
        this->commandsExecuteEndpoint(req, res);
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
            {"playerDetails", info.playerDetails},
            {"lobbyPlayerDetails", info.lobbyPlayerDetails},
            {"playerStats", info.playerStats},
            {"inGamePlayers", info.inGamePlayers},
            {"bannedPlayers", info.bannedPlayers},
            {"serverStatus", info.connectedClients >= 0 ? "Online" : "Offline"},
            {"tps", info.tps}
        };

        res.set_content(jsonResponse.dump(), "application/json");
    } catch (const std::exception &) {
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

void rserv::HttpServer::configEndpoint(const httplib::Request &req, httplib::Response &res) {
    if (!this->checkAuth(req)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
    }

    try {
        nlohmann::json jsonResponse = {
            {"serverIp", _serverConfig->getIp()},
            {"httpPort", constants::HTTP_SERVER_PORT},
            {"gamePort", _serverConfig->getPort()},
            {"tps", _serverConfig->getTps()}
        };

        res.set_content(jsonResponse.dump(), "application/json");
    } catch (const std::exception &) {
        res.status = 500;
        res.set_content("Internal server error", "text/plain");
    }
}

void rserv::HttpServer::commandsSuggestionsEndpoint(
    const httplib::Request &req,
    httplib::Response &res
) {
    if (!this->checkAuth(req)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
    }

    try {
        nlohmann::json jsonResponse = {
            {"suggestions", {
                "/close <lobby_id or lobby_code>",
                "/kick <player_id or player_name>",
                "/ban <player_id or player_name>",
                "/unban <player_id or player_name>",
                "/tps <value>"
            }}
        };

        res.set_content(jsonResponse.dump(), "application/json");
    } catch (const std::exception &) {
        res.status = 500;
        res.set_content("Internal server error", "text/plain");
    }
}

void rserv::HttpServer::commandsExecuteEndpoint(
    const httplib::Request &req,
    httplib::Response &res
) {
    if (!this->checkAuth(req)) {
        res.status = 401;
        res.set_content("Unauthorized", "text/plain");
        return;
    }

    try {
        auto jsonBody = nlohmann::json::parse(req.body);
        std::string command = jsonBody["command"];

        std::string message = _commandExecutor(command);

        nlohmann::json jsonResponse = {
            {"message", message}
        };

        res.set_content(jsonResponse.dump(), "application/json");
    } catch (const std::exception &) {
        res.status = 400;
        res.set_content("Invalid request", "text/plain");
    }
}
