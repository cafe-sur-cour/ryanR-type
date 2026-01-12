/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** HttpServer
*/

#ifndef HTTPSERVER_HPP_
#define HTTPSERVER_HPP_

#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <string>
#include <httplib.h>
#include <chrono>

#include "../ServerConfig.hpp"

namespace rserv {

struct ServerInfo {
    int connectedClients;
    std::chrono::seconds uptime;
    int activeLobbies;
    size_t totalPlayers;
    std::vector<std::string> lobbyDetails;
    std::vector<std::string> playerDetails;
    std::vector<std::vector<std::string>> lobbyPlayerDetails;
};

class HttpServer {
    public:
        HttpServer(
            std::function<bool()> statusChecker,
            std::function<ServerInfo()> infoGetter,
            std::shared_ptr<ServerConfig> serverConfig
        );
        ~HttpServer();

        void start();
        void stop();

        void statusEndpoint(const httplib::Request &, httplib::Response &res);
        void infoEndpoint(const httplib::Request &, httplib::Response &res);
        void configEndpoint(const httplib::Request &, httplib::Response &res);

    private:
        void httpLoop();
        void loadEnv();
        bool checkAuth(const httplib::Request &req);

        std::thread _httpThread;
        std::atomic_bool _running;
        std::function<bool()> _statusChecker;
        std::function<ServerInfo()> _infoGetter;
        std::shared_ptr<ServerConfig> _serverConfig;
        std::unique_ptr<httplib::Server> _server;
        std::string _password;
};

}  // namespace rserv

#endif /* !HTTPSERVER_HPP_ */
