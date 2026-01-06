/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicComponent registration
*/

#include "MusicComponent.hpp"
#include "../../../common/Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../../common/constants.hpp"

REGISTER_COMPONENT(
    ecs::MusicComponent,
    constants::MUSICCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::MUSICFILE_FIELD, FieldType::STRING},
        Field{constants::INITIALSTATEMUSIC_FIELD, FieldType::STRING},
        Field{constants::VOLUME_FIELD, FieldType::FLOAT},
        Field{constants::LOOP_FIELD, FieldType::BOOL}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto musicFile = std::get<std::string>(*fields.at(constants::MUSICFILE_FIELD));
        auto initialStateStr = std::get<std::string>(*fields.at(constants::INITIALSTATEMUSIC_FIELD));
        auto volume = std::get<float>(*fields.at(constants::VOLUME_FIELD));
        auto loop = std::get<bool>(*fields.at(constants::LOOP_FIELD));
        
        ecs::MusicState initialState = ecs::STOPPED;
        if (initialStateStr == constants::PLAYING_FIELD) initialState = ecs::PLAYING;
        else if (initialStateStr == constants::PAUSED_FIELD) initialState = ecs::PAUSED;
        else if (initialStateStr == constants::CHANGING_FIELD) initialState = ecs::CHANGING;
        else if (initialStateStr == constants::STOPPED_FIELD) initialState = ecs::STOPPED;
        
        return std::make_shared<ecs::MusicComponent>(musicFile, initialState, volume, loop);
    }
)
