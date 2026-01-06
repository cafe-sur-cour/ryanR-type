/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundIntentComponent registration
*/

#include "SoundIntentComponent.hpp"
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::SoundIntentComponent,
    constants::SOUNDINTENTCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::SOUND_FILE_FIELD, FieldType::STRING},
        Field{constants::VOLUME_FIELD, FieldType::FLOAT, true, 
              std::make_shared<FieldValue>(100.0f)}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto soundFile = std::get<std::string>(*fields.at(constants::SOUND_FILE_FIELD));
        auto volume = std::get<float>(*fields.at(constants::VOLUME_FIELD));
        return std::make_shared<ecs::SoundIntentComponent>(soundFile, volume);
    }
)
