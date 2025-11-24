/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_HPP_
    #define SERVER_HPP_

#include <memory>
#include <asio.hpp>
#include "IServer.hpp"
#include "ServerConfig.hpp"


namespace rserv {
    class Server : public IServer {
        public:
            Server(unsigned int port);
            ~Server();

            void init() override;
            void start() override;
            void stop() override;

            operator int() const noexcept override;

            std::shared_ptr<ServerConfig> getConfig() const override;
            int getState() const override;
            int getFd() const override;
            unsigned int getPort() const override;

            void setState(int state) override;
            void setFd(int fd) override;
            void setPort(unsigned int port) override;

        private:
            std::shared_ptr<ServerConfig> _config;
    };
} // namespace rserv = r-type server

#endif /* !SERVER_HPP_ */
