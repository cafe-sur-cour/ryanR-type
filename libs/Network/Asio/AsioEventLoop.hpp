/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AsioEventLoop - Implémentation ASIO de IEventLoop
*/

#ifndef ASIOEVENTLOOP_HPP_
#define ASIOEVENTLOOP_HPP_

#include "../IEventLoop.hpp"
#include <asio.hpp>
#include <memory>

namespace net {

class AsioEventLoop : public IEventLoop {
    public:
        AsioEventLoop();
        ~AsioEventLoop() override = default;

        void run() override;
        void runOne() override;
        void stop() override;
        bool stopped() const override;
        void post(std::function<void()> task) override;
        void restart() override;

        asio::io_context& getIoContext() { return _ioContext; }
        const asio::io_context& getIoContext() const { return _ioContext; }

    private:
        asio::io_context _ioContext;
};

class EventLoopFactory {
    public:
        static std::shared_ptr<IEventLoop> create();
};

} // namespace net

#endif /* !ASIOEVENTLOOP_HPP_ */
