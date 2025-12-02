/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundSystem
*/

#ifndef SOUNDSYSTEM_HPP_
#define SOUNDSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class SoundSystem : public ASystem {
    public:
        SoundSystem();
        ~SoundSystem() override = default;

    protected:
        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

} // namespace ecs

#endif /* !SOUNDSYSTEM_HPP_ */
