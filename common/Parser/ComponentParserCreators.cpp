/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComponentCreators
*/

#include "Parser.hpp"

void Parser::instanciateComponentDefinitions() {
    std::map<std::string, std::pair<std::type_index, std::vector<Field>>> componentDefinitions = {
        {"TransformComponent", {std::type_index(typeid(ecs::TransformComponent)), {
            {"position", FieldType::VECTOR2F},
            {"scale", FieldType::VECTOR2F},
            {"rotation", FieldType::FLOAT}
        }}},
        {"VelocityComponent", {std::type_index(typeid(ecs::VelocityComponent)), {}}},
        {"SpeedComponent", {std::type_index(typeid(ecs::SpeedComponent)), {
            {"speed", FieldType::FLOAT}
        }}},
        {"SpriteComponent", {std::type_index(typeid(ecs::SpriteComponent)), {
            {"filePath", FieldType::STRING}
        }}},
        {"AnimationComponent", {std::type_index(typeid(ecs::AnimationComponent)), {
            {"animationPath", FieldType::STRING},
            {"frameWidth", FieldType::FLOAT},
            {"frameHeight", FieldType::FLOAT},
            {"frameCount", FieldType::INT},
            {"startWidth", FieldType::FLOAT},
            {"startHeight", FieldType::FLOAT}
        }}},
        {"ControllableTag", {std::type_index(typeid(ecs::ControllableTag)), {}}},
        {"PlayerTag", {std::type_index(typeid(ecs::PlayerTag)), {}}},
        {"ColliderComponent", {std::type_index(typeid(ecs::ColliderComponent)), {
            {"size", FieldType::VECTOR2F}
        }}}
    };
    _componentDefinitions = componentDefinitions;
}

void Parser::instanciateComponentCreators() {
    _componentCreators = {
        {std::type_index(typeid(ecs::TransformComponent)), [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            auto pos = std::get<math::Vector2f>(*fields.at("position"));
            auto scale = std::get<math::Vector2f>(*fields.at("scale"));
            auto rot = std::get<float>(*fields.at("rotation"));
            return std::make_shared<ecs::TransformComponent>(pos, rot, scale);
        }},
        {std::type_index(typeid(ecs::VelocityComponent)), []([[maybe_unused]] const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            return std::make_shared<ecs::VelocityComponent>();
        }},
        {std::type_index(typeid(ecs::SpeedComponent)), [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            auto speed = std::get<float>(*fields.at("speed"));
            return std::make_shared<ecs::SpeedComponent>(speed);
        }},
        {std::type_index(typeid(ecs::SpriteComponent)), [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            auto path = std::get<std::string>(*fields.at("filePath"));
            return std::make_shared<ecs::SpriteComponent>(path);
        }},
        {std::type_index(typeid(ecs::AnimationComponent)), [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            auto animPath = std::get<std::string>(*fields.at("animationPath"));
            auto fw = std::get<float>(*fields.at("frameWidth"));
            auto fh = std::get<float>(*fields.at("frameHeight"));
            auto fc = std::get<int>(*fields.at("frameCount"));
            auto sw = std::get<float>(*fields.at("startWidth"));
            auto sh = std::get<float>(*fields.at("startHeight"));
            return std::make_shared<ecs::AnimationComponent>(animPath, fw, fh, fc, sw, sh);
        }},
        {std::type_index(typeid(ecs::ControllableTag)), []([[maybe_unused]] const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            return std::make_shared<ecs::ControllableTag>();
        }},
        {std::type_index(typeid(ecs::PlayerTag)), []([[maybe_unused]] const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            return std::make_shared<ecs::PlayerTag>();
        }},
        {std::type_index(typeid(ecs::ColliderComponent)), [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
            auto size = std::get<math::Vector2f>(*fields.at("size"));
            return std::make_shared<ecs::ColliderComponent>(math::Vector2f(0.0f, 0.0f), size);
        }}
    };
}

void Parser::instanciateComponentAdders() {
    _componentAdders = {
        {std::type_index(typeid(ecs::TransformComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::TransformComponent>(component));
        }},
        {std::type_index(typeid(ecs::VelocityComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::VelocityComponent>(component));
        }},
        {std::type_index(typeid(ecs::SpeedComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::SpeedComponent>(component));
        }},
        {std::type_index(typeid(ecs::AnimationComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::AnimationComponent>(component));
        }},
        {std::type_index(typeid(ecs::SpriteComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::SpriteComponent>(component));
        }},
        {std::type_index(typeid(ecs::PlayerTag)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::PlayerTag>(component));
        }},
        {std::type_index(typeid(ecs::ControllableTag)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::ControllableTag>(component));
        }},
        {std::type_index(typeid(ecs::ColliderComponent)), [](std::shared_ptr<ecs::Registry> registry, ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
            registry->addComponent(entity, std::static_pointer_cast<ecs::ColliderComponent>(component));
        }}
    };
}