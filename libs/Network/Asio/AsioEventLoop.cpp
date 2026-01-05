/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AsioEventLoop implementation
*/

#include <utility>
#include <memory>

#include "AsioEventLoop.hpp"

net::AsioEventLoop::AsioEventLoop() : _ioContext() {
}

void net::AsioEventLoop::run() {
    _ioContext.run();
}

void net::AsioEventLoop::runOne() {
    _ioContext.run_one();
}

void net::AsioEventLoop::stop() {
    _ioContext.stop();
}

bool net::AsioEventLoop::stopped() const {
    return _ioContext.stopped();
}

void net::AsioEventLoop::post(std::function<void()> task) {
    asio::post(_ioContext, std::move(task));
}

void net::AsioEventLoop::restart() {
    _ioContext.restart();
}

std::shared_ptr<net::IEventLoop> net::EventLoopFactory::create() {
    return std::make_shared<net::AsioEventLoop>();
}
