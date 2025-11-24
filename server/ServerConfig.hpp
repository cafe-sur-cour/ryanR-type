/*
** EPITECH PROJECT, 2025
** Header
** File description:
** Header
*/

#ifndef SERVER_CONFIG_HPP_
    #define SERVER_CONFIG_HPP_

namespace rserv {
    class ServerConfig {
        public:
            ServerConfig(unsigned int port);
            ~ServerConfig();

            int getState() const;
            int getFd() const;
            unsigned int getPort() const;

            void setState(int state);
            void setFd(int fd);
            void setPort(unsigned int port);

        private:
            int _state;
            int _fd;
            unsigned int _port;
    };
} // namespace rserv = r-type server

#endif /* !SERVER_CONFIG_HPP_ */
