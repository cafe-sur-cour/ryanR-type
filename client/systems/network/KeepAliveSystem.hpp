/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** KeepAliveSystem
*/

#ifndef KEEPALIVESYSTEM_HPP_
#define KEEPALIVESYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <chrono>

namespace ecs {

class KeepAliveSystem : public ASystem {
    public:
        KeepAliveSystem();
        ~KeepAliveSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        std::chrono::steady_clock::time_point _lastPingTime;
        const float _pingInterval;
};

}

#endif /* !KEEPALIVESYSTEM_HPP_ */
