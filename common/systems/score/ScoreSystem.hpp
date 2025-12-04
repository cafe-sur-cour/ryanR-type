/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ScoreSystem
*/

#ifndef SCORESYSTEM_HPP_
#define SCORESYSTEM_HPP_

#include "../base/ASystem.hpp"

namespace ecs {

class ScoreSystem : public ASystem {
    public:
        ScoreSystem();
        ~ScoreSystem();
        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;
    protected:
    private:
};

}  // namespace ecs

#endif /* !SCORESYSTEM_HPP_ */
