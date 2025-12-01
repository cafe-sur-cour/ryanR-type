/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PlayerPrefab
*/

#ifndef PLAYERPREFAB_HPP_
#define PLAYERPREFAB_HPP_

#include "../APrefab.hpp"
#include "../../components/permanent/TransformComponent.hpp"
#include "../../components/permanent/VelocityComponent.hpp"
#include "../../components/permanent/SpeedComponent.hpp"
#include "../../../client/components/rendering/SpriteComponent.hpp"
#include "../../../client/components/rendering/AnimationComponent.hpp"
#include "../../components/tags/ControllableTag.hpp"
#include "../../components/tags/PlayerTag.hpp"
#include "../../components/permanent/ColliderComponent.hpp"
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

        ecs::Entity instantiate(const std::shared_ptr<ecs::Registry> &registry) override {
            ecs::Entity entity = registry->createEntity();
            auto transform = std::make_shared<ecs::TransformComponent>(math::Vector2f(_x, _y));
            transform->setScale(math::Vector2f(_scale, _scale));
            registry->addComponent(entity, transform);
            registry->addComponent(entity, std::make_shared<ecs::VelocityComponent>());
            registry->addComponent(entity, std::make_shared<ecs::SpeedComponent>());
            registry->addComponent(entity,
                std::make_shared<ecs::AnimationComponent>(_animationPath, _frameWidth, _frameHeight, _frameCount, _startWidth, _startHeight));
            registry->addComponent(entity, std::make_shared<ecs::PlayerTag>());
            registry->addComponent(entity, std::make_shared<ecs::ControllableTag>());
            auto collider = std::make_shared<ecs::ColliderComponent>(
                math::Vector2f(0.0f, 0.0f),
                math::Vector2f(100.0f, 100.0f),
                ecs::CollisionType::Solid
            );
            registry->addComponent(entity, collider);
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
