/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerPrefab
*/

#ifndef PLAYERPREFAB_HPP_
#define PLAYERPREFAB_HPP_

#include "../APrefab.hpp"
#include "../../ECS/component/permanent/TransformComponent.hpp"
#include "../../ECS/component/permanent/VelocityComponent.hpp"
#include "../../ECS/component/permanent/SpeedComponent.hpp"
#include "../../ECS/component/permanent/SpriteComponent.hpp"
#include "../../ECS/component/permanent/AnimationComponent.hpp"
#include "../../ECS/component/tags/ControllableTag.hpp"
#include "../../ECS/component/tags/PlayerTag.hpp"
#include "../../types/Vector2f.hpp"
#include <memory>
#include <string>
#include <iostream>


class PlayerPrefab : public APrefab {
    public:
        PlayerPrefab(float x, float y, float scale,
                    const std::string &animationPath,
                    float frameWidth, float frameHeight,
                    float startWidth, float startHeight,
                    int frameCount)
            : _x(x), _y(y), _scale(scale),
              _animationPath(animationPath),
              _frameWidth(frameWidth),
              _frameHeight(frameHeight),
              _startWidth(startWidth),
              _startHeight(startHeight),
              _frameCount(frameCount) {}

        ~PlayerPrefab() = default;

        size_t instantiate(const std::shared_ptr<ecs::ARegistry> &registry) override {
            size_t entity = registry->createEntity();
            auto transform = std::make_shared<ecs::TransformComponent>(math::Vector2f(_x, _y));
            transform->setScale(math::Vector2f(_scale, _scale));
            registry->addComponent(entity, transform);
            registry->addComponent(entity, std::make_shared<ecs::VelocityComponent>());
            registry->addComponent(entity, std::make_shared<ecs::SpeedComponent>());
            registry->addComponent(entity,
                std::make_shared<ecs::AnimationComponent>(_animationPath, _frameWidth, _frameHeight, _frameCount, _startWidth, _startHeight));
            registry->addComponent(entity, std::make_shared<ecs::PlayerTag>());
            registry->addComponent(entity, std::make_shared<ecs::ControllableTag>());
            return entity;
        }

    private:
        float _x;
        float _y;
        float _scale;
        std::string _animationPath;
        float _frameWidth, _frameHeight;
        float _startWidth;
        float _startHeight;
        int _frameCount;
};

#endif /* !PLAYERPREFAB_HPP_ */
