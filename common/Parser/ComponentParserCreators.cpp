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
#include <functional>
#include "Parser.hpp"
#include "../constants.hpp"
#include "../components/tags/ShooterTag.hpp"
#include "../components/tags/ProjectileTag.hpp"
#include "../components/tags/ProjectilePassThroughTag.hpp"
#include "../components/permanent/ShootingStatsComponent.hpp"
#include "../components/permanent/ProjectilePrefabComponent.hpp"
#include "../components/permanent/VelocityComponent.hpp"
#include "../../client/components/rendering/RectangleRenderComponent.hpp"
#include "../components/permanent/LifetimeComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../ECS/entity/Entity.hpp"
#include "../ECS/entity/registry/Registry.hpp"

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
            {constants::STATES_FIELD, FieldType::JSON},
            {constants::INITIALSTATE_FIELD, FieldType::STRING},
            {constants::TRANSITIONS_FIELD, FieldType::JSON}
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
        {constants::PROJECTILEPASSTHROUGHTAG,
            {std::type_index(typeid(ecs::ProjectilePassThroughTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::COLLIDERCOMPONENT, {std::type_index(typeid(ecs::ColliderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SIZE_FIELD, FieldType::VECTOR2F}
        }}},
        {constants::SHOOTINGSTATSCOMPONENT,
            {std::type_index(typeid(ecs::ShootingStatsComponent)), {
                {constants::TARGET_FIELD, FieldType::STRING},
                {constants::FIRERATE_FIELD, FieldType::FLOAT},
                {constants::PROJECTILESPEED_FIELD, FieldType::FLOAT},
                {constants::SHOTCOUNT_FIELD, FieldType::INT},
                {constants::ANGLEOFFSET_FIELD, FieldType::FLOAT},
                {constants::SPREADANGLE_FIELD, FieldType::FLOAT}
            }
        }},
        {constants::RECTANGLERENDERCOMPONENT, {
            std::type_index(typeid(ecs::RectangleRenderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::WIDTH_FIELD, FieldType::FLOAT},
            {constants::HEIGHT_FIELD, FieldType::FLOAT},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}},
        {constants::PROJECTILEPREFABCOMPONENT, {
            std::type_index(typeid(ecs::ProjectilePrefabComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::PREFABNAME_FIELD, FieldType::STRING}
        }}},
        {constants::LIFETIMECOMPONENT, {
            std::type_index(typeid(ecs::LifetimeComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::LIFETIME_FIELD, FieldType::FLOAT}
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
        auto statesJson = std::get<nlohmann::json>(*fields.at(constants::STATES_FIELD));
        auto initialState = std::get<std::string>(*fields.at(constants::INITIALSTATE_FIELD));

        auto anim = std::make_shared<ecs::AnimationComponent>();

        for (auto& [stateName, stateData] : statesJson.items()) {
            std::string texturePath = stateData[constants::TEXTUREPATH_FIELD];
            float frameWidth = stateData[constants::FRAMEWIDTH_FIELD];
            float frameHeight = stateData[constants::FRAMEHEIGHT_FIELD];
            int frameCount = stateData[constants::FRAMECOUNT_FIELD];
            float startWidth = stateData[constants::STARTWIDTH_FIELD];
            float startHeight = stateData[constants::STARTHEIGHT_FIELD];
            float speed = stateData.value(constants::SPEED_FIELD, 0.1f);
            bool loop = stateData.value(constants::LOOP_FIELD, true);

            ecs::AnimationClip clip{
                texturePath, frameWidth, frameHeight,
                frameCount, startWidth, startHeight, speed, loop};
            anim->addState(stateName, std::make_shared<ecs::AnimationClip>(clip));
        }

        anim->setCurrentState(initialState);

        if (fields.count(constants::TRANSITIONS_FIELD)) {
            auto transitionsJson =
                std::get<nlohmann::json>(*fields.at(constants::TRANSITIONS_FIELD));
            for (auto& transitionData : transitionsJson) {
                std::string from = transitionData[constants::FROM_FIELD];
                std::string to = transitionData[constants::TO_FIELD];
                bool rewind = transitionData[constants::REWIND_FIELD];

                std::vector<ecs::AnimationCondition> conditions;
                auto conditionsJson = transitionData[constants::CONDITIONS_FIELD];
                for (auto& condData : conditionsJson) {
                    std::string param = condData[constants::PARAM_FIELD];
                    bool equals = condData[constants::EQUALS_FIELD];
                    conditions.push_back({param, equals});
                }

                anim->addTransition(from, to, conditions, rewind);
            }
        }

        return anim;
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

    registerComponent<
    ecs::ProjectilePassThroughTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ProjectilePassThroughTag>();
    });

    registerComponent<ecs::ColliderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto size = std::get<math::Vector2f>(*fields.at(constants::SIZE_FIELD));
        return std::make_shared<ecs::ColliderComponent>(math::Vector2f(0.0f, 0.0f), size);
    });

    registerComponent<ecs::ShootingStatsComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto fireRate = std::get<float>(*fields.at(constants::FIRERATE_FIELD));
        auto projectileSpeed = std::get<float>(*fields.at(constants::PROJECTILESPEED_FIELD));
        auto shotCount = std::get<int>(*fields.at(constants::SHOTCOUNT_FIELD));
        auto angleOffset = std::get<float>(*fields.at(constants::ANGLEOFFSET_FIELD));
        auto spreadAngle = std::get<float>(*fields.at(constants::SPREADANGLE_FIELD));
        ecs::MultiShotPattern pattern(shotCount, spreadAngle, angleOffset);
        return std::make_shared<ecs::ShootingStatsComponent>(
            fireRate, projectileSpeed, pattern
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

    registerComponent<ecs::ProjectilePrefabComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto prefabName = std::get<std::string>(*fields.at(constants::PREFABNAME_FIELD));
        return std::make_shared<ecs::ProjectilePrefabComponent>(prefabName);
    });

    registerComponent<ecs::LifetimeComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto lifetime = std::get<float>(*fields.at(constants::LIFETIME_FIELD));
        return std::make_shared<ecs::LifetimeComponent>(lifetime);
    });
}

template<typename T>
void Parser::registerComponent(const ComponentCreator& creator) {
    std::type_index idx(typeid(T));
    _componentCreators[idx] = creator;
    _componentAdders[idx] = [](const std::shared_ptr<ecs::Registry>& registry,
        ecs::Entity entity, std::shared_ptr<ecs::IComponent> component) {
        auto originalComponent = std::static_pointer_cast<T>(component);
        auto clonedComponent = std::make_shared<T>(*originalComponent);
        registry->addComponent(entity, clonedComponent);
    };
}
