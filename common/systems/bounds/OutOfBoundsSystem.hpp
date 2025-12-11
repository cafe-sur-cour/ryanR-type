/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** OutOfBoundsSystem
*/

#ifndef OUTOFBOUNDSSYSTEM_HPP_
#define OUTOFBOUNDSSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class OutOfBoundsSystem : public ASystem {
    public:
        OutOfBoundsSystem();
        ~OutOfBoundsSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        float _margin;
};

}

#endif /* !OUTOFBOUNDSSYSTEM_HPP_ */
