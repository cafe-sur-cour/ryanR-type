/*
** EPITECH PROJECT, 2025
** Header
** File description:
** Header
*/

#ifndef SERVER_CONFIG_HPP_
    #define SERVER_CONFIG_HPP_

#include <cstdint>
#include <string>

namespace rserv {
    class ServerConfig {
        public:
            ServerConfig();
            ~ServerConfig();

            int getState() const;

            void setPort(uint16_t port);
            uint16_t getPort() const;

            void setState(int state);

            std::string getIp() const;
            void setIp(std::string ip);

            void setIsDebug(bool isDebug);
            bool getIsDebug() const;
        private:
            int _state;
            uint16_t _port;
            std::string _ip;
            bool _isDebug;
    };
} // namespace rserv = r-type server

#endif /* !SERVER_CONFIG_HPP_ */
