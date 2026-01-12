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

rserv::HttpServer::HttpServer(std::function<bool()> statusChecker) :
    _running(false),
    _statusChecker(statusChecker),
    _server(nullptr) {
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

void rserv::HttpServer::statusEndpoint(const httplib::Request &, httplib::Response &res) {
    if (_statusChecker()) {
        res.set_content("Online", "text/plain");
    } else {
        res.set_content("Offline", "text/plain");
    }
}

void rserv::HttpServer::httpLoop() {
    _server = std::make_unique<httplib::Server>();

    _server->Get("/api/status", [this](const httplib::Request &req, httplib::Response &res) {
        this->statusEndpoint(req, res);
    });
    _server->set_mount_point("/", "./frontend/dist");
    std::cout << "HTTP server starting on port 8080" << std::endl;
    _server->listen("0.0.0.0", 8080);
}
