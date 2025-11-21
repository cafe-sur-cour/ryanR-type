/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IServer
*/

#ifndef ISERVER_HPP_
#define ISERVER_HPP_

namespace net {

class IServer {
    public:
        virtual ~IServer() = default;
        virtual void start() = 0;
        virtual void stop() = 0;
};

} // namespace net

#endif /* !ISERVER_HPP_ */
