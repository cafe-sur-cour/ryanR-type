/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Constants
*/

#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_

#include <cstdint>
#include <string>
#include "types/Vector2f.hpp"

namespace constants {
    /* Network Defaults */
    constexpr int DEFAULT_SERVER_PORT = 4242;
    const std::string DEFAULT_SERVER_IP = "127.0.0.1";  // 127.0.0.1

    /* Game Defaults */
    constexpr float BASE_SPEED = 100.0f;
    constexpr float EPS = 1e-6f;
    constexpr float PLAYER_BASE_SPEED = 300.0f;
    constexpr float GAMEPAD_DEADZONE = 0.15f;
    constexpr float AXIS_MAX_VALUE = 100.0f;
    constexpr int SMOOTH_MOVEMENT_ITERATIONS = 4;

    /* Texture Defaults */
    constexpr float DEFAULT_TEXTURE_WIDTH = 1920.0f;
    constexpr float DEFAULT_TEXTURE_HEIGHT = 1080.0f;

    enum class EventType {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        SHOOT = 4,
    };
    constexpr int MAX_RETRY_CONNECTIONS = 3;
    /* Paths */
    const std::string CONFIG_PATH = "configs/entities/";

    /* Parsing constants */
    const std::string SERVER_VALUE = "server";
    const std::string CLIENT_VALUE = "client";
    const std::string BOTH_VALUE = "both";

    const std::string TRANSFORMCOMPONENT = "TransformComponent";
    const std::string VELOCITYCOMPONENT = "VelocityComponent";
    const std::string SPEEDCOMPONENT = "SpeedComponent";
    const std::string SPRITECOMPONENT = "SpriteComponent";
    const std::string ANIMATIONCOMPONENT = "AnimationComponent";
    const std::string CONTROLLABLETAG = "ControllableTag";
    const std::string PLAYERTAG = "PlayerTag";
    const std::string COLLIDERCOMPONENT = "ColliderComponent";
    const std::string SHOOTERTAG = "ShooterTag";
    const std::string PROJECTILETAG = "ProjectileTag";
    const std::string SHOOTINGSTATSCOMPONENT = "ShootingStatsComponent";
    const std::string RECTANGLERENDERCOMPONENT = "RectangleRenderComponent";
    const std::string PROJECTILEPREFABCOMPONENT = "ProjectilePrefabComponent";
    const std::string TEXTCOMPONENT = "TextComponent";
    const std::string PROJECTILEPASSTHROUGHTAG = "ProjectilePassThroughTag";
    const std::string PARALLAXCOMPONENT = "ParallaxComponent";

    const std::string SCORETAG = "ScoreTag";
    const std::string SCORECOMPONENT = "ScoreComponent";
    const std::string SCORE_FIELD = "score";
    const std::string TARGET_FIELD = "target";
    const std::string POSITION_FIELD = "position";
    const std::string SCALE_FIELD = "scale";
    const std::string ROTATION_FIELD = "rotation";
    const std::string SPEED_FIELD = "speed";
    const std::string FILEPATH_FIELD = "filePath";
    const std::string ANIMATIONPATH_FIELD = "animationPath";
    const std::string FRAMEWIDTH_FIELD = "frameWidth";
    const std::string FRAMEHEIGHT_FIELD = "frameHeight";
    const std::string FRAMECOUNT_FIELD = "frameCount";
    const std::string STARTWIDTH_FIELD = "startWidth";
    const std::string STARTHEIGHT_FIELD = "startHeight";
    const std::string SIZE_FIELD = "size";
    const std::string FIRERATE_FIELD = "fireRate";
    const std::string PROJECTILESPEED_FIELD = "projectileSpeed";
    const std::string SHOTCOUNT_FIELD = "shotCount";
    const std::string ANGLEOFFSET_FIELD = "angleOffset";
    const std::string SPREADANGLE_FIELD = "spreadAngle";
    const std::string WIDTH_FIELD = "width";
    const std::string HEIGHT_FIELD = "height";
    const std::string COLOR_FIELD = "color";
    const std::string R_FIELD = "r";
    const std::string G_FIELD = "g";
    const std::string B_FIELD = "b";

    const std::string STATES_FIELD = "states";
    const std::string INITIALSTATE_FIELD = "initialState";
    const std::string TRANSITIONS_FIELD = "transitions";
    const std::string CONDITIONS_FIELD = "conditions";
    const std::string CONDITION_FIELD = "condition";
    const std::string PARAM_FIELD = "param";
    const std::string EQUALS_FIELD = "equals";
    const std::string FROM_FIELD = "from";
    const std::string BASESCROLLSPEED_FIELD = "baseScrollSpeed";
    const std::string DIRECTION_FIELD = "direction";
    const std::string LAYERS_FIELD = "layers";
    const std::string NAME_FIELD = "name";
    const std::string SPEEDMULTIPLIER_FIELD = "speedMultiplier";
    const std::string SCALEMODE_FIELD = "scaleMode";
    const std::string SOURCESIZE_FIELD = "sourceSize";
    const std::string REPEAT_FIELD = "repeat";
    const std::string ZINDEX_FIELD = "zIndex";
    const std::string TO_FIELD = "to";
    const std::string REWIND_FIELD = "rewind";
    const std::string TEXTUREPATH_FIELD = "texturePath";
    const std::string LOOP_FIELD = "loop";
    const std::string SCALEMODE_FITSCREEN = "FIT_SCREEN";
    const std::string SCALEMODE_STRETCH = "STRETCH";
    const std::string SCALEMODE_MANUAL = "MANUAL";

    const std::string X_FIELD = "x";
    const std::string Y_FIELD = "y";

    const std::string PREFABNAME_FIELD = "prefabName";
    const std::string LIFETIMECOMPONENT = "LifetimeComponent";
    const std::string LIFETIME_FIELD = "lifetime";
    const std::string BACKGROUNDMUSICTAG = "BackGroundMusicTag";
    const std::string TEXT_FIELD = "text";
    const std::string FONTPATH_FIELD = "fontPath";

    const std::string MUSICCOMPONENT = "MusicComponent";
    const std::string MUSICFILE_FIELD = "musicFile";
    const std::string VOLUME_FIELD = "volume";
    const std::string INITIALSTATEMUSIC_FIELD = "initialState";
    const std::string PLAYING_FIELD = "PLAYING";
    const std::string PAUSED_FIELD = "PAUSED";
    const std::string STOPPED_FIELD = "STOPPED";
    const std::string CHANGING_FIELD = "CHANGING";
    const math::Vector2f TILE_SIZE = math::Vector2f(64.0f, 64.0f);

    const float MAX_HEIGHT = 1080.0f;
    const float MAX_WIDTH = 1920.0f;
    const float GAME_ZONE_BOUNDARY_THICKNESS = 100.0f;

    /* Map parsing constants */
    const std::string MUSIC_FIELD = "music";
    const std::string BACKGROUND_FIELD = "background";
    const std::string BACKGROUND_SCROLL_SPEED_FIELD = "scrollSpeed";
    const std::string TILE_FIELD = "tile";
    const std::string LEGEND_FIELD = "legend";
    const std::string MAP_FIELD = "map";
    const std::string WAVES_FIELD = "waves";
    const std::string SPAWNLENGTH_FIELD = "spawnLength";
    const std::string POSX_FIELD = "posX";
    const std::string ENEMIES_FIELD = "enemies";
    const std::string TYPE_FIELD = "type";
    const std::string COUNT_FIELD = "count";
    const std::string EMPTY_PREFAB = "empty";
    const math::Vector2f BACKGROUND_POSITION = math::Vector2f(0.0f, 0.0f);
    const float BACKGROUND_PARALLAX_DIRECTION_X = -1.0f;
    const float BACKGROUND_PARALLAX_DIRECTION_Y = 0.0f;

    /* Animation conditions */
    const std::string VELOCITY_UP_CONDITION = "isVelocityUp";
    const std::string VELOCITY_DOWN_CONDITION = "isVelocityDown";
}

#endif /* !CONSTANTS_HPP_ */
