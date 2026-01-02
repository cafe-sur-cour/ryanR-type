/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AsioEventLoop implementation
*/

#include "AsioEventLoop.hpp"

namespace net {

AsioEventLoop::AsioEventLoop() : _ioContext() {
}

void AsioEventLoop::run() {
    _ioContext.run();
}

void AsioEventLoop::runOne() {
    _ioContext.run_one();
}

void AsioEventLoop::stop() {
    _ioContext.stop();
}

bool AsioEventLoop::stopped() const {
    return _ioContext.stopped();
}

void AsioEventLoop::post(std::function<void()> task) {
    asio::post(_ioContext, std::move(task));
}

void AsioEventLoop::restart() {
    _ioContext.restart();
}

// Factory implementation
std::shared_ptr<IEventLoop> EventLoopFactory::create() {
    return std::make_shared<AsioEventLoop>();
}

} // namespace net
