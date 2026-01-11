/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MapGeneratorSystem
*/

#ifndef MAPGENERATORSYSTEM_HPP_
#define MAPGENERATORSYSTEM_HPP_

#include "../base/ASystem.hpp"
#include <random>

namespace ecs {

class MapGeneratorSystem : public ASystem {
    public:
        MapGeneratorSystem(unsigned int seed = 42);
        ~MapGeneratorSystem() = default;

        void update(
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry,
            float deltaTime
        ) override;

    private:
        void generateObstaclesAt(
            float x,
            std::shared_ptr<ResourceManager> resourceManager,
            std::shared_ptr<Registry> registry
        );
        float noise(float x);
        unsigned int _seed;
        std::mt19937 _rng;
        float _lastGeneratedX;
        const float _generationStep;
        const float _startGenerationX;
};

}

#endif /* !MAPGENERATORSYSTEM_HPP_ */
