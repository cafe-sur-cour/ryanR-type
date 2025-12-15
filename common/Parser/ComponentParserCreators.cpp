/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ComponentCreators
*/

#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include "Parser.hpp"
#include "../constants.hpp"
#include "../components/tags/ControllableTag.hpp"
#include "../components/tags/PlayerTag.hpp"
#include "../components/tags/ShooterTag.hpp"
#include "../components/tags/PlayerProjectileTag.hpp"
#include "../components/tags/EnnemyProjectileTag.hpp"
#include "../components/tags/MobTag.hpp"
#include "../components/tags/ObstacleTag.hpp"
#include "../components/tags/GameZoneColliderTag.hpp"
#include "../components/permanent/GameZoneComponent.hpp"
#include "../components/tags/ProjectilePassThroughTag.hpp"
#include "../components/permanent/ShootingStatsComponent.hpp"
#include "../components/permanent/ProjectilePrefabComponent.hpp"
#include "../components/permanent/AIMovementPatternComponent.hpp"
#include "../components/tags/AIMoverTag.hpp"
#include "../components/tags/AIShooterTag.hpp"
#include "../components/permanent/VelocityComponent.hpp"
#include "../../client/components/rendering/RectangleRenderComponent.hpp"
#include "../../client/components/rendering/TextComponent.hpp"
#include "../../client/components/rendering/HealthBarComponent.hpp"
#include "../components/permanent/LifetimeComponent.hpp"
#include "../../client/components/rendering/AnimationComponent.hpp"
#include "../../client/components/rendering/MusicComponent.hpp"
#include "../../client/components/rendering/ParallaxComponent.hpp"
#include "../../client/components/temporary/SoundIntentComponent.hpp"
#include "../components/permanent/InteractionConfigComponent.hpp"
#include "../components/permanent/HealthComponent.hpp"
#include "../ECS/entity/Entity.hpp"
#include "../ECS/entity/registry/Registry.hpp"
#include "../../client/components/tags/BackGroundMusicTag.hpp"
#include "../components/permanent/ScoreComponent.hpp"
#include "../components/permanent/ScoreValueComponent.hpp"
#include "../components/permanent/DamageComponent.hpp"

void Parser::instanciateComponentDefinitions() {
    std::map<std::string, std::pair<std::type_index,
    std::vector<Field>>> componentDefinitions = {
        {constants::TRANSFORMCOMPONENT, {std::type_index(typeid(ecs::TransformComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::POSITION_FIELD, FieldType::VECTOR2F},
            {constants::SCALE_FIELD, FieldType::VECTOR2F,
                true, std::make_shared<FieldValue>(math::Vector2f(1.0f, 1.0f))},
            {constants::ROTATION_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(0.0f)}
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
            {constants::TRANSITIONS_FIELD, FieldType::JSON,
                true, std::make_shared<FieldValue>(nlohmann::json::array())}
        }}},
        {constants::SOUNDINTENTCOMPONENT,
            {std::type_index(typeid(ecs::SoundIntentComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SOUND_FILE_FIELD, FieldType::STRING},
            {constants::VOLUME_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(100.0f)}
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
        {constants::PLAYERPROJECTILETAG, {std::type_index(typeid(ecs::PlayerProjectileTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::ENNEMYPROJECTILETAG, {std::type_index(typeid(ecs::EnnemyProjectileTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::MOBTAG, {std::type_index(typeid(ecs::MobTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::OBSTACLETAG, {std::type_index(typeid(ecs::ObstacleTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::GAMEZONECOLLIDERTAG, {std::type_index(typeid(ecs::GameZoneColliderTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::PROJECTILEPASSTHROUGHTAG,
            {std::type_index(typeid(ecs::ProjectilePassThroughTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::AIMOVERTAG, {std::type_index(typeid(ecs::AIMoverTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::AISHOOTERTAG, {std::type_index(typeid(ecs::AIShooterTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::COLLIDERCOMPONENT, {std::type_index(typeid(ecs::ColliderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::OFFSET_FIELD, FieldType::VECTOR2F,
                true, std::make_shared<FieldValue>(math::Vector2f(0.0f, 0.0f))},
            {constants::SIZE_FIELD, FieldType::VECTOR2F},
            {constants::TYPE_FIELD, FieldType::STRING}
        }}},
        {constants::SHOOTINGSTATSCOMPONENT,
            {std::type_index(typeid(ecs::ShootingStatsComponent)), {
                {constants::TARGET_FIELD, FieldType::STRING},
                {constants::FIRERATE_FIELD, FieldType::FLOAT},
                {constants::SHOTCOUNT_FIELD, FieldType::INT,
                    true, std::make_shared<FieldValue>(1)},
                {constants::ANGLEOFFSET_FIELD, FieldType::FLOAT,
                    true, std::make_shared<FieldValue>(0.0f)},
                {constants::SPREADANGLE_FIELD, FieldType::FLOAT,
                    true, std::make_shared<FieldValue>(0.0f)}
            }
        }},
        {constants::RECTANGLERENDERCOMPONENT, {
            std::type_index(typeid(ecs::RectangleRenderComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::WIDTH_FIELD, FieldType::FLOAT},
            {constants::HEIGHT_FIELD, FieldType::FLOAT},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}},
        {constants::TEXTCOMPONENT, {std::type_index(typeid(ecs::TextComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::TEXT_FIELD, FieldType::STRING},
            {constants::FONTPATH_FIELD, FieldType::STRING},
            {constants::COLOR_FIELD, FieldType::OBJECT}
        }}},
        {constants::HEALTHBARCOMPONENT, {std::type_index(typeid(ecs::HealthBarComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING}
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
        }}},
        {constants::LIFESPANCOMPONENT, {
            std::type_index(typeid(ecs::LifetimeComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::LIFESPAN_FIELD, FieldType::FLOAT}
        }}},
        {constants::PARALLAXCOMPONENT, {
            std::type_index(typeid(ecs::ParallaxComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::BASESCROLLSPEED_FIELD, FieldType::FLOAT},
            {constants::DIRECTION_FIELD, FieldType::VECTOR2F},
            {constants::LAYERS_FIELD, FieldType::JSON}
        }}},
        {constants::GAMEZONECOMPONENT, {std::type_index(typeid(ecs::GameZoneComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::ZONERECT_FIELD, FieldType::JSON}
        }}},
                {constants::BACKGROUNDMUSICTAG, {
            std::type_index(typeid(ecs::BackGroundMusicTag)), {
            {constants::TARGET_FIELD, FieldType::STRING}
        }}},
        {constants::MUSICCOMPONENT, {
            std::type_index(typeid(ecs::MusicComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::MUSICFILE_FIELD, FieldType::STRING},
            {constants::INITIALSTATEMUSIC_FIELD, FieldType::STRING},
            {constants::VOLUME_FIELD, FieldType::FLOAT},
            {constants::LOOP_FIELD, FieldType::BOOL}
        }}},
        {constants::SCORECOMPONENT, {
            std::type_index(typeid(ecs::ScoreComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SCORE_FIELD, FieldType::INT}
        }}},
        {constants::SCOREVALUECOMPONENT, {
            std::type_index(typeid(ecs::ScoreValueComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::SCOREVALUE_FIELD, FieldType::INT}
        }}},
        {constants::DAMAGECOMPONENT, {
            std::type_index(typeid(ecs::DamageComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::DAMAGE_FIELD, FieldType::FLOAT}
        }}},
        {constants::HEALTHCOMPONENT, {
            std::type_index(typeid(ecs::HealthComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::HEALTH_FIELD, FieldType::FLOAT}
        }}},
        {constants::INTERACTIONCONFIGCOMPONENT, {
            std::type_index(typeid(ecs::InteractionConfigComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::MAPPINGS_FIELD, FieldType::JSON}
        }}},
        {constants::AIMOVEMENTPATTERNCOMPONENT, {
            std::type_index(typeid(ecs::AIMovementPatternComponent)), {
            {constants::TARGET_FIELD, FieldType::STRING},
            {constants::DEFAULTBEHAVIOR_FIELD, FieldType::STRING},
            {constants::ZIGZAGAMPLITUDE_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(constants::DEFAULT_ZIGZAG_AMPLITUDE)},
            {constants::ZIGZAGFREQUENCY_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(constants::DEFAULT_ZIGZAG_FREQUENCY)},
            {constants::DETECTIONRANGE_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(constants::DEFAULT_DETECTION_RANGE)},
            {constants::VERTICALDEADZONE_FIELD, FieldType::FLOAT,
                true, std::make_shared<FieldValue>(constants::DEFAULT_VERTICAL_DEADZONE)},
        }}},
    };
    _componentDefinitions = std::make_shared<std::map<std::string,
        std::pair<std::type_index, std::vector<Field>>>>(std::move(componentDefinitions));
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

    registerComponent<ecs::SoundIntentComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto soundFile = std::get<std::string>(*fields.at(constants::SOUND_FILE_FIELD));
        auto volume = std::get<float>(*fields.at(constants::VOLUME_FIELD));
        return std::make_shared<ecs::SoundIntentComponent>(soundFile, volume);
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

    registerComponent<ecs::PlayerProjectileTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::PlayerProjectileTag>();
    });

    registerComponent<ecs::EnnemyProjectileTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::EnnemyProjectileTag>();
    });

    registerComponent<ecs::MobTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::MobTag>();
    });

    registerComponent<ecs::ObstacleTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ObstacleTag>();
    });

    registerComponent<ecs::GameZoneColliderTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::GameZoneColliderTag>();
    });

    registerComponent<ecs::GameZoneComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto zoneRectJson = std::get<nlohmann::json>(*fields.at(constants::ZONERECT_FIELD));
        math::FRect zoneRect(zoneRectJson[constants::X_FIELD],
            zoneRectJson[constants::Y_FIELD],
            zoneRectJson[constants::WIDTH_FIELD], zoneRectJson[constants::HEIGHT_FIELD]);
        return std::make_shared<ecs::GameZoneComponent>(zoneRect);
    });

    registerComponent<
    ecs::ProjectilePassThroughTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::ProjectilePassThroughTag>();
    });

    registerComponent<ecs::ColliderComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto offset = std::get<math::Vector2f>(*fields.at(constants::OFFSET_FIELD));
        auto size = std::get<math::Vector2f>(*fields.at(constants::SIZE_FIELD));
        auto typeStr = std::get<std::string>(*fields.at(constants::TYPE_FIELD));

        ecs::CollisionType type = ecs::CollisionType::Solid;
        if (typeStr == constants::COLLISION_TYPE_SOLID) {
            type = ecs::CollisionType::Solid;
        } else if (typeStr == constants::COLLISION_TYPE_TRIGGER) {
            type = ecs::CollisionType::Trigger;
        } else if (typeStr == constants::COLLISION_TYPE_PUSH) {
            type = ecs::CollisionType::Push;
        } else if (typeStr == constants::COLLISION_TYPE_NONE) {
            type = ecs::CollisionType::None;
        }

        std::vector<std::vector<std::string>> includeTags;
        std::vector<std::vector<std::string>> excludeTags;

        return std::make_shared<ecs::ColliderComponent>(
            offset, size, type);
    });

    registerComponent<ecs::ShootingStatsComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto fireRate = std::get<float>(*fields.at(constants::FIRERATE_FIELD));
        auto shotCount = std::get<int>(*fields.at(constants::SHOTCOUNT_FIELD));
        auto angleOffset = std::get<float>(*fields.at(constants::ANGLEOFFSET_FIELD));
        auto spreadAngle = std::get<float>(*fields.at(constants::SPREADANGLE_FIELD));
        ecs::MultiShotPattern pattern(shotCount, spreadAngle, angleOffset);
        return std::make_shared<ecs::ShootingStatsComponent>(
            fireRate, pattern
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

    registerComponent<ecs::TextComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto text = std::get<std::string>(*fields.at(constants::TEXT_FIELD));
        auto fontPath = std::get<std::string>(*fields.at(constants::FONTPATH_FIELD));
        auto colorObj = std::get<std::map<std::string, std::shared_ptr<FieldValue>>>
            (*fields.at(constants::COLOR_FIELD));
        auto r = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::R_FIELD)));
        auto g = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::G_FIELD)));
        auto b = static_cast<uint8_t>(std::get<int>(*colorObj.at(constants::B_FIELD)));
        gfx::color_t color = {r, g, b};
        return std::make_shared<ecs::TextComponent>(text, fontPath, color);
    });

    registerComponent<ecs::HealthBarComponent>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::HealthBarComponent>();
    });

    registerComponent<ecs::ProjectilePrefabComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto prefabName = std::get<std::string>(*fields.at(constants::PREFABNAME_FIELD));
        return std::make_shared<ecs::ProjectilePrefabComponent>(prefabName);
    });

    registerComponent<ecs::LifetimeComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        float lifetime = 0.0f;
        if (fields.find(constants::LIFETIME_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFETIME_FIELD));
        } else if (fields.find(constants::LIFESPAN_FIELD) != fields.end()) {
            lifetime = std::get<float>(*fields.at(constants::LIFESPAN_FIELD));
        }
        return std::make_shared<ecs::LifetimeComponent>(lifetime);
    });
    registerComponent<ecs::ScoreComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto score = std::get<int>(*fields.at(constants::SCORE_FIELD));
        return std::make_shared<ecs::ScoreComponent>(score);
    });
    registerComponent<ecs::ScoreValueComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto scoreValue = std::get<int>(*fields.at(constants::SCOREVALUE_FIELD));
        return std::make_shared<ecs::ScoreValueComponent>(scoreValue);
    });
    registerComponent<ecs::ParallaxComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto parallax = std::make_shared<ecs::ParallaxComponent>();

        auto baseSpeed = std::get<float>(*fields.at(constants::BASESCROLLSPEED_FIELD));
        auto direction = std::get<math::Vector2f>(*fields.at(constants::DIRECTION_FIELD));
        auto layersJson = std::get<nlohmann::json>(*fields.at(constants::LAYERS_FIELD));

        parallax->setBaseScrollSpeed(baseSpeed);
        parallax->setDirection(direction);

        for (const auto& layerJson : layersJson) {
            ecs::ParallaxLayer layer;

            layer.name = layerJson.value(constants::NAME_FIELD, "");
            layer.filePath = layerJson.value(constants::FILEPATH_FIELD, "");
            layer.speedMultiplier = layerJson.value(constants::SPEEDMULTIPLIER_FIELD, 1.0f);

            if (layerJson.contains(constants::SCALE_FIELD)) {
                auto scaleJson = layerJson[constants::SCALE_FIELD];
                layer.scale = math::Vector2f(
                    scaleJson.value(constants::X_FIELD, 1.0f),
                    scaleJson.value(constants::Y_FIELD, 1.0f)
                );
            }

            if (layerJson.contains(constants::SCALEMODE_FIELD)) {
                std::string scaleModeStr = layerJson[constants::SCALEMODE_FIELD];
                if (scaleModeStr == constants::SCALEMODE_FITSCREEN) {
                    layer.scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                } else if (scaleModeStr == constants::SCALEMODE_STRETCH) {
                    layer.scaleMode = ecs::ParallaxScaleMode::STRETCH;
                } else if (scaleModeStr == constants::SCALEMODE_MANUAL) {
                    layer.scaleMode = ecs::ParallaxScaleMode::MANUAL;
                } else {
                    layer.scaleMode = ecs::ParallaxScaleMode::FIT_SCREEN;
                }
            }

            if (layerJson.contains(constants::SOURCESIZE_FIELD)) {
                auto sourceSizeJson = layerJson[constants::SOURCESIZE_FIELD];
                layer.sourceSize = math::Vector2f(
                    sourceSizeJson.value(constants::X_FIELD, constants::DEFAULT_TEXTURE_WIDTH),
                    sourceSizeJson.value(constants::Y_FIELD, constants::DEFAULT_TEXTURE_HEIGHT)
                );
            }

            layer.repeat = layerJson.value(constants::REPEAT_FIELD, true);
            layer.zIndex = layerJson.value(constants::ZINDEX_FIELD, 0);
            layer.currentOffset = math::Vector2f(0.0f, 0.0f);

            parallax->addLayer(layer);
        }

        parallax->sortLayersByZIndex();
        return parallax;
    });
    registerComponent<ecs::BackGroundMusicTag>([]([[maybe_unused]]
        const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::BackGroundMusicTag>();
    });
    registerComponent<ecs::MusicComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto musicFile = std::get<std::string>(*fields.at(constants::MUSICFILE_FIELD));
        auto initialStateStr = std::get<std::string>
            (*fields.at(constants::INITIALSTATEMUSIC_FIELD));
        auto volume = std::get<float>(*fields.at(constants::VOLUME_FIELD));
        auto loop = std::get<bool>(*fields.at(constants::LOOP_FIELD));
        ecs::MusicState initialState = ecs::STOPPED;
        if (initialStateStr == constants::PLAYING_FIELD) initialState = ecs::PLAYING;
        else if (initialStateStr == constants::PAUSED_FIELD) initialState = ecs::PAUSED;
        else if (initialStateStr == constants::CHANGING_FIELD) initialState = ecs::CHANGING;
        else if (initialStateStr == constants::STOPPED_FIELD) initialState = ecs::STOPPED;
        return std::make_shared<ecs::MusicComponent>(musicFile, initialState, volume, loop);
    });

    registerComponent<ecs::ScoreComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto score = std::get<int>(*fields.at(constants::SCORE_FIELD));
        return std::make_shared<ecs::ScoreComponent>(score);
    });

    registerComponent<ecs::DamageComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto damage = std::get<float>(*fields.at(constants::DAMAGE_FIELD));
        return std::make_shared<ecs::DamageComponent>(damage);
    });

    registerComponent<ecs::HealthComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto health = std::get<float>(*fields.at(constants::HEALTH_FIELD));
        return std::make_shared<ecs::HealthComponent>(health);
    });

    registerComponent<ecs::InteractionConfigComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto mappingsJson = std::get<nlohmann::json>(*fields.at(constants::MAPPINGS_FIELD));
        std::vector<ecs::InteractionMapping> mappings;

        for (const auto& interaction : mappingsJson) {
            ecs::InteractionMapping mapping;
            if (interaction.contains(constants::TAGS_FIELD)) {
                for (const auto& tag : interaction[constants::TAGS_FIELD]) {
                    mapping.targetTags.push_back(tag);
                }
            }
            if (interaction.contains(constants::TOENTITY_FIELD)) {
                auto toEntityValue = interaction[constants::TOENTITY_FIELD];
                if (toEntityValue.is_array()) {
                    for (const auto& action : toEntityValue) {
                        mapping.actionsToOther.push_back(action);
                    }
                } else if (toEntityValue.is_string()) {
                    mapping.actionsToOther.push_back(toEntityValue);
                }
            }
            if (interaction.contains(constants::TOSELF_FIELD)) {
                auto toSelfValue = interaction[constants::TOSELF_FIELD];
                if (toSelfValue.is_array()) {
                    for (const auto& action : toSelfValue) {
                        mapping.actionsToSelf.push_back(action);
                    }
                } else if (toSelfValue.is_string()) {
                    mapping.actionsToSelf.push_back(toSelfValue);
                }
            }
            mappings.push_back(mapping);
        }
        return std::make_shared<ecs::InteractionConfigComponent>(mappings);
    });
    registerComponent<ecs::AIMoverTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::AIMoverTag>();
    });
    registerComponent<ecs::AIShooterTag>([]([[maybe_unused]] const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        return std::make_shared<ecs::AIShooterTag>();
    });
    registerComponent<ecs::AIMovementPatternComponent>([](const std::map<std::string,
        std::shared_ptr<FieldValue>>& fields) -> std::shared_ptr<ecs::IComponent> {
        auto behaviorStr = std::get<std::string>(*fields.at(constants::DEFAULTBEHAVIOR_FIELD));
        auto zigzagAmplitude = std::get<float>(*fields.at(constants::ZIGZAGAMPLITUDE_FIELD));
        auto zigzagFrequency = std::get<float>(*fields.at(constants::ZIGZAGFREQUENCY_FIELD));
        auto detectionRange = std::get<float>(*fields.at(constants::DETECTIONRANGE_FIELD));
        auto verticalDeadzone = std::get<float>(*fields.at(constants::VERTICALDEADZONE_FIELD));

        ecs::AIMovementPattern pattern = ecs::AIMovementPattern::STRAIGHT_LINE;
        if (behaviorStr == constants::STRAIGHT_LINE_VALUE) {
            pattern = ecs::AIMovementPattern::STRAIGHT_LINE;
        } else if (behaviorStr == constants::ZIGZAG_VALUE) {
            pattern = ecs::AIMovementPattern::ZIGZAG;
        } else if (behaviorStr == constants::VERTICAL_MIRROR_VALUE) {
            pattern = ecs::AIMovementPattern::VERTICAL_MIRROR;
        } else if (behaviorStr == constants::FOLLOW_RIGHT_VALUE) {
            pattern = ecs::AIMovementPattern::FOLLOW_RIGHT;
        }

        auto component = std::make_shared<ecs::AIMovementPatternComponent>();
        component->setPattern(pattern);
        component->setZigzagAmplitude(zigzagAmplitude);
        component->setZigzagFrequency(zigzagFrequency);
        component->setDetectionRange(detectionRange);
        component->setVerticalDeadzone(verticalDeadzone);
        component->setTimer(0.0f);
        return component;
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
