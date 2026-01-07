/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationComponent
*/

#include "AnimationComponent.hpp"
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::AnimationComponent,
    constants::ANIMATIONCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::STATES_FIELD, FieldType::JSON},
        Field{constants::INITIALSTATE_FIELD, FieldType::STRING},
        Field{constants::TRANSITIONS_FIELD, FieldType::JSON, true,
              std::make_shared<FieldValue>(nlohmann::json::array())}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
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
            anim->addState(stateName,
                std::make_shared<ecs::AnimationClip>(clip));
        }

        anim->setCurrentState(initialState);

        if (fields.count(constants::TRANSITIONS_FIELD)) {
            auto transitionsJson = std::get<nlohmann::json>(
                *fields.at(constants::TRANSITIONS_FIELD));
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
    }
)
