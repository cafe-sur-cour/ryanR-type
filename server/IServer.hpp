/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef ISERVER_HPP_
    #define ISERVER_HPP_

#include <memory>
#include "ServerConfig.hpp"

namespace net {
    class IServer {
        public:
            virtual ~IServer() = default;

            virtual void init() = 0;
            virtual void start() = 0;
            virtual void stop() = 0;

            virtual operator int() const noexcept = 0;

            virtual std::shared_ptr<ServerConfig> getConfig() const = 0;
            virtual int getState() const = 0;
            virtual int getFd() const = 0;
            virtual unsigned int getPort() const = 0;

            virtual void setState(int state) = 0;
            virtual void setFd(int fd) = 0;
            virtual void setPort(unsigned int port) = 0;
    };
} // namespace net
#endif /* !ISERVER_HPP_ */
