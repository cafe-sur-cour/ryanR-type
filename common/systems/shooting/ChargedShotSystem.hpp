/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ChargedShotSystem
*/

#ifndef CHARGEDSHOTSYSTEM_HPP_
#define CHARGEDSHOTSYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class ChargedShotSystem : public ASystem {
    public:
        ChargedShotSystem();
        ~ChargedShotSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
};

}

#endif /* !CHARGEDSHOTSYSTEM_HPP_ */
