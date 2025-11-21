/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "IServer.hpp"

namespace net {

class Server : public IServer {
    public:
        Server();
        ~Server();

        void start() override;
        void stop() override;

    protected:
    private:
};

} // namespace net

#endif /* !SERVER_HPP_ */
