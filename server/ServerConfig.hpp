/*
** EPITECH PROJECT, 2025
** Header
** File description:
** Header
*/

#ifndef SERVER_CONFIG_HPP_
    #define SERVER_CONFIG_HPP_

namespace net {

class ServerConfig {
    public:
        /* Constructor and Destructor */
        ServerConfig(unsigned int port);
        ~ServerConfig();

        /* Getters */
        int getState() const;
        int getFd() const;
        unsigned int getPort() const;

        /* Setters */
        void setState(int state);
        void setFd(int fd);
        void setPort(unsigned int port);

    private:
        int _state;
        int _fd;
        unsigned int _port;
};

#endif /* !SERVER_CONFIG_HPP_ */
