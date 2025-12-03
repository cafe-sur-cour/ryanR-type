/*
** EPITECH PROJECT, 2025
** Header
** File description:
** Header
*/


#include <cstdint>
#include <string>

#ifndef SERVER_CONFIG_HPP_
    #define SERVER_CONFIG_HPP_

namespace rserv {
    class ServerConfig {
        public:
            ServerConfig();
            ~ServerConfig();

            int getState() const;
            int getFd() const;

            void setPort(uint16_t port);
            uint16_t getPort() const;

            void setState(int state);
            void setFd(int fd);

            void setNbClients(int nbClients);
            int getNbClients() const;

            std::string getIp() const;
            void setIp(std::string ip);

        private:
            int _state;
            int _fd;
            uint16_t _port;
            int _nbClients;
            std::string _ip;
    };
} // namespace rserv = r-type server

#endif /* !SERVER_CONFIG_HPP_ */
