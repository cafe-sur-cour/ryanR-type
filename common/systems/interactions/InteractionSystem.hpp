/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** InteractionSystem
*/

#ifndef INTERACTIONSYSTEM_HPP_
#define INTERACTIONSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <memory>
#include <string>
#include <vector>

namespace ecs {
    class Registry;
}

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

    private:
        std::vector<std::string> filterDamageActions(
            const std::vector<std::string>& actions,
            std::shared_ptr<Registry> registry,
            Entity targetEntity
        );
};

}  // namespace ecs


#endif /* !INTERACTIONSYSTEM_HPP_ */
