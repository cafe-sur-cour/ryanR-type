/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AsioEventLoop - Implémentation ASIO de IEventLoop
*/

#ifndef ASIOEVENTLOOP_HPP_
#define ASIOEVENTLOOP_HPP_

#include "IEventLoop.hpp"
#include <asio.hpp>
#include <memory>

namespace net {

/**
 * @brief Implémentation ASIO de l'interface IEventLoop
 * Encapsule asio::io_context pour permettre son remplacement
 */
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

    /**
     * @brief Accès à l'io_context sous-jacent
     * @note Utilisé uniquement par les implémentations réseau ASIO internes
     * Ne doit JAMAIS être exposé au code métier
     */
    asio::io_context& getIoContext() { return _ioContext; }
    const asio::io_context& getIoContext() const { return _ioContext; }

private:
    asio::io_context _ioContext;
};

/**
 * @brief Factory pour créer une EventLoop selon la configuration
 * Permet de changer l'implémentation en un seul point
 */
class EventLoopFactory {
public:
    static std::shared_ptr<IEventLoop> create();
};

} // namespace net

#endif /* !ASIOEVENTLOOP_HPP_ */
