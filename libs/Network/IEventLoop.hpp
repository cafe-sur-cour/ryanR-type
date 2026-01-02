/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IEventLoop
*/

#ifndef IEVENTLOOP_HPP_
#define IEVENTLOOP_HPP_

#include <functional>
#include <memory>

namespace net {

class IEventLoop {
    public:
        virtual ~IEventLoop() = default;
        virtual void run() = 0;
        virtual void runOne() = 0;
        virtual void stop() = 0;
        virtual bool stopped() const = 0;
        virtual void post(std::function<void()> task) = 0;
        virtual void restart() = 0;
    };

} // namespace net

#endif /* !IEVENTLOOP_HPP_ */
