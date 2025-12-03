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

    enum class EventType {
        UP = 0,
        DOWN = 1,
        LEFT = 2,
        RIGHT = 3,
        SHOOT = 4,
    };
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

    const std::string X_FIELD = "x";
    const std::string Y_FIELD = "y";

    const std::string PREFABNAME_FIELD = "prefabName";
}

#endif /* !CONSTANTS_HPP_ */
