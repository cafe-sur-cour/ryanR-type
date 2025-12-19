/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ClientEffectCleanupSystem
*/

#ifndef CLIENTEFFECTCLEANUPSYSTEM_HPP_
#define CLIENTEFFECTCLEANUPSYSTEM_HPP_

#include "../../common/systems/base/ASystem.hpp"

namespace ecs {

class ClientEffectCleanupSystem : public ASystem {
    public:
        ClientEffectCleanupSystem();
        ~ClientEffectCleanupSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}

#endif /* !CLIENTEFFECTCLEANUPSYSTEM_HPP_ */
