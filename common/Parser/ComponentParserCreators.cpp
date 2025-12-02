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
#include "../components/tags/ShooterTag.hpp"
#include "../components/tags/ProjectileTag.hpp"
#include "../components/permanent/ShootingStatsComponent.hpp"
#include "../../client/components/rendering/RectangleRenderComponent.hpp"

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
        {constants::SHOOTERTAG, {std::type_index(typeid(ecs::ShooterTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PROJECTILETAG, {std::type_index(typeid(ecs::ProjectileTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::COLLIDERCOMPONENT, {std::type_index(typeid(ecs::ColliderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SIZE_FIELD, FieldType::VECTOR2F}
        }}},
        {constants::SHOOTINGSTATSCOMPONENT,
            {std::type_index(typeid(ecs::ShootingStatsComponent)), {
                {constants::TARGET_FIELD, FieldType::STRING},
                {constants::FIRERATE_FIELD, FieldType::FLOAT, true},
                {constants::PROJECTILESPEED_FIELD, FieldType::FLOAT, true},
                {constants::SHOTCOUNT_FIELD, FieldType::INT, true},
                {constants::ANGLEOFFSET_FIELD, FieldType::FLOAT, true},
                {constants::SPREADANGLE_FIELD, FieldType::FLOAT, true},
                {constants::SHOOTINGSTATSREF_FIELD, FieldType::STRING, true}
            }
        }},
        {constants::RECTANGLERENDERCOMPONENT, {std::type_index(typeid(ecs::RectangleRenderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::WIDTH_FIELD, FieldType::FLOAT},
            {constants::HEIGHT_FIELD, FieldType::FLOAT},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}}
    };
    _componentDefinitions = std::make_shared<std::map<std::string,
        std::pair<std::type_index, std::vector<Field>>>>(componentDefinitions);
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

    registerComponent<ecs::ShooterTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ShooterTag>();
    });

    registerComponent<ecs::ProjectileTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ProjectileTag>();
    });

    registerComponent<ecs::ColliderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto size = std::get<math::Vector2f>(*fields.at(constants::SIZE_FIELD));
        return std::make_shared<ecs::ColliderComponent>(math::Vector2f(0.0f, 0.0f), size);
    });

    registerComponent<ecs::ShootingStatsComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {

        if (fields.find(constants::SHOOTINGSTATSREF_FIELD) != fields.end()) {
            auto statsName = std::get<std::string>(*fields.at(constants::SHOOTINGSTATSREF_FIELD));
            auto component = std::make_shared<ecs::ShootingStatsComponent>();
            return component;
        }

        auto fireRate = std::get<float>(*fields.at(constants::FIRERATE_FIELD));
        auto projectileSpeed = std::get<float>(*fields.at(constants::PROJECTILESPEED_FIELD));
        auto shotCount = std::get<int>(*fields.at(constants::SHOTCOUNT_FIELD));
        auto angleOffset = std::get<float>(*fields.at(constants::ANGLEOFFSET_FIELD));
        auto spreadAngle = std::get<float>(*fields.at(constants::SPREADANGLE_FIELD));
        ecs::MultiShotPattern pattern(shotCount, spreadAngle, angleOffset);
        return std::make_shared<ecs::ShootingStatsComponent>(
            fireRate, nullptr, projectileSpeed, pattern
        );
    });

    registerComponent<ecs::RectangleRenderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto width = std::get<float>(*fields.at(constants::WIDTH_FIELD));
        auto height = std::get<float>(*fields.at(constants::HEIGHT_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::RectangleRenderComponent>(color, width, height);
    });
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
