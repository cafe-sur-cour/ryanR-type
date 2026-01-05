/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AsioEventLoop
*/

#ifndef ASIOEVENTLOOP_HPP_
#define ASIOEVENTLOOP_HPP_

#include <asio.hpp>
#include <memory>
#include "../../../common/interfaces/IEventLoop.hpp"

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

        std::shared_ptr<asio::io_context> getIoContext() { return std::shared_ptr<asio::io_context>(&_ioContext, [](asio::io_context*){}); }
        std::shared_ptr<const asio::io_context> getIoContext() const { return std::shared_ptr<const asio::io_context>(&_ioContext, [](const asio::io_context*){}); }

    private:
        asio::io_context _ioContext;
};

class EventLoopFactory {
    public:
        static std::shared_ptr<IEventLoop> create();
};

} // namespace net

#endif /* !ASIOEVENTLOOP_HPP_ */
