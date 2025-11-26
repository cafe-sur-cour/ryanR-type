/*
** EPITECH PROJECT, 2025
** Header
** File description:
** Header
*/

#include <cstdint>

#ifndef SERVER_CONFIG_HPP_
    #define SERVER_CONFIG_HPP_

namespace rserv {
    class ServerConfig {
        public:
            ServerConfig();
            ~ServerConfig();

            int getState() const;
            int getFd() const;

            void setPort(unsigned int port);
            unsigned int getPort() const;

            void setState(int state);
            void setFd(int fd);

            void setNbClients(int nbClients);
            int getNbClients() const;

            uint32_t getIp() const;
            void setIp(uint32_t ip);

        private:
            int _state;
            int _fd;
            unsigned int _port;
            int _nbClients;
            uint32_t _ip;
    };
} // namespace rserv = r-type server

#endif /* !SERVER_CONFIG_HPP_ */
