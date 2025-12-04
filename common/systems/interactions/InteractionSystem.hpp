/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem
*/

#ifndef INTERACTIONSYSTEM_HPP_
#define INTERACTIONSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class InteractionSystem : public ASystem {
    public:
        InteractionSystem();
        ~InteractionSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

}  // namespace ecs

#endif /* !INTERACTIONSYSTEM_HPP_ */
