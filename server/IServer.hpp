/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef ISERVER_HPP_
    #define ISERVER_HPP_

class IServer {
    public:
        virtual ~IServer() = default;

        virtual void init() = 0;
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual operator int() const noexcept = 0;
};

#endif /* !ISERVER_HPP_ */
