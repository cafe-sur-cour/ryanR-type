/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationStateSyncSystem
*/

#ifndef ANIMATIONSTATESYNCSYSTEM_HPP_
#define ANIMATIONSTATESYNCSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <unordered_map>
#include <string>

namespace ecs {

class AnimationStateSyncSystem : public ASystem {
    public:
        AnimationStateSyncSystem() = default;
        ~AnimationStateSyncSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
                   std::shared_ptr<Registry> registry,
                   float deltaTime) override;
};

}  // namespace ecs

#endif  // ANIMATIONSTATESYNCSYSTEM_HPP_
