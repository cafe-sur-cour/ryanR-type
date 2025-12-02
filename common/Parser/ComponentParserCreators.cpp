/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComponentCreators
*/

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include "Parser.hpp"
#include "../constants.hpp"

void Parser::instanciateComponentDefinitions() {
    std::map<std::string, std::pair<std::type_index,
    std::vector<Field>>> componentDefinitions = {
        {constants::TRANSFORMCOMPONENT, {std::type_index(typeid(ecs::TransformComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::POSITION_FIELD, FieldType::VECTOR2F},
            {constants::SCALE_FIELD, FieldType::VECTOR2F},
            {constants::ROTATION_FIELD, FieldType::FLOAT}
        }}},
        {constants::VELOCITYCOMPONENT, {std::type_index(typeid(ecs::VelocityComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::SPEEDCOMPONENT, {std::type_index(typeid(ecs::SpeedComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SPEED_FIELD, FieldType::FLOAT}
        }}},
        {constants::SPRITECOMPONENT, {std::type_index(typeid(ecs::SpriteComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::FILEPATH_FIELD, FieldType::STRING}
        }}},
        {constants::ANIMATIONCOMPONENT, {std::type_index(typeid(ecs::AnimationComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::ANIMATIONPATH_FIELD, FieldType::STRING},
            {constants::FRAMEWIDTH_FIELD, FieldType::FLOAT},
            {constants::FRAMEHEIGHT_FIELD, FieldType::FLOAT},
            {constants::FRAMECOUNT_FIELD, FieldType::INT},
            {constants::STARTWIDTH_FIELD, FieldType::FLOAT},
            {constants::STARTHEIGHT_FIELD, FieldType::FLOAT}
        }}},
        {constants::CONTROLLABLETAG, {std::type_index(typeid(ecs::ControllableTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PLAYERTAG, {std::type_index(typeid(ecs::PlayerTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::COLLIDERCOMPONENT, {std::type_index(typeid(ecs::ColliderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SIZE_FIELD, FieldType::VECTOR2F}
        }}}
    };
    _componentDefinitions = std::make_shared<std::map<std::string, std::pair<std::type_index, std::vector<Field>>>>(componentDefinitions);
}

void Parser::instanciateComponentCreators() {
    registerComponent<ecs::TransformComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto pos = std::get<math::Vector2f>(*fields.at(constants::POSITION_FIELD));
        auto scale = std::get<math::Vector2f>(*fields.at(constants::SCALE_FIELD));
        auto rot = std::get<float>(*fields.at(constants::ROTATION_FIELD));
        return std::make_shared<ecs::TransformComponent>(pos, rot, scale);
    });

    registerComponent<ecs::VelocityComponent>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::VelocityComponent>();
    });

    registerComponent<ecs::SpeedComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto speed = std::get<float>(*fields.at(constants::SPEED_FIELD));
        return std::make_shared<ecs::SpeedComponent>(speed);
    });

    registerComponent<ecs::SpriteComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto path = std::get<std::string>(*fields.at(constants::FILEPATH_FIELD));
        return std::make_shared<ecs::SpriteComponent>(path);
    });

    registerComponent<ecs::AnimationComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto animPath = std::get<std::string>(*fields.at(constants::ANIMATIONPATH_FIELD));
        auto fw = std::get<float>(*fields.at(constants::FRAMEWIDTH_FIELD));
        auto fh = std::get<float>(*fields.at(constants::FRAMEHEIGHT_FIELD));
        auto fc = std::get<int>(*fields.at(constants::FRAMECOUNT_FIELD));
        auto sw = std::get<float>(*fields.at(constants::STARTWIDTH_FIELD));
        auto sh = std::get<float>(*fields.at(constants::STARTHEIGHT_FIELD));
        return std::make_shared<ecs::AnimationComponent>(animPath, fw, fh, fc, sw, sh);
    });

    registerComponent<ecs::ControllableTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ControllableTag>();
    });

    registerComponent<ecs::PlayerTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::PlayerTag>();
    });

    registerComponent<ecs::ColliderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto size = std::get<math::Vector2f>(*fields.at(constants::SIZE_FIELD));
        return std::make_shared<ecs::ColliderComponent>(math::Vector2f(0.0f, 0.0f), size);
    });
}

template<typename T>
void Parser::componentAdders(const std::shared_ptr<ecs::Registry>& registry,
    ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
    registry->addComponent(entity, std::static_pointer_cast<T>(component));
}

template<typename T>
void Parser::registerComponent(const ComponentCreator& creator) {
    std::type_index idx(typeid(T));
    _componentCreators[idx] = creator;
    _componentAdders[idx] = [](const std::shared_ptr<ecs::Registry>& registry,
        ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
        registry->addComponent(entity, std::static_pointer_cast<T>(component));
    };
}
