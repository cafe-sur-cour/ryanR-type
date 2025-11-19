/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#include <memory>
#include "IServer.hpp"
#include "ServerConfig.hpp"

class Server : public IServer {
    public:
        Server(unsigned int port);
        ~Server();

        void init() override;
        void start() override;
        void stop() override;
        operator int() const noexcept override;

        std::shared_ptr<ServerConfig> getConfig();
        int getState() const;
        int getFd() const;
        unsigned int getPort() const;

    private:
        std::shared_ptr<ServerConfig> _config;
};

#endif /* !SERVER_HPP_ */
