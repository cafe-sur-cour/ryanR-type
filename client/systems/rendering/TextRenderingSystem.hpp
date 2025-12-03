/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** TextRenderingSystem
*/

#ifndef TEXTRENDERINGSYSTEM_HPP_
#define TEXTRENDERINGSYSTEM_HPP_

#include "../../../common/systems/base/ASystem.hpp"
#include <memory>

namespace ecs {

class TextRenderingSystem : public ASystem {
    public:
        TextRenderingSystem();
        ~TextRenderingSystem() override = default;

    protected:
        void update(std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry, float deltaTime) override;
};

} // namespace ecs

#endif /* !TEXTRENDERINGSYSTEM_HPP_ */
