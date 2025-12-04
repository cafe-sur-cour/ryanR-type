/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicSystem
*/

#ifndef MUSICSYSTEM_HPP_
#define MUSICSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class MusicSystem : public ASystem {
    public:
        MusicSystem();
        ~MusicSystem() override = default;

    protected:
        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
};

} // namespace ecs

#endif /* !MUSICSYSTEM_HPP_ */