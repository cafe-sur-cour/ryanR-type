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
    const std::string DEFAULT_SERVER_IP = "127.0.0.1";

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
        STOP = 5,
        FORCE = 6,
        HEALTHCHECK = 7,
    };

    constexpr char END_OFSTRING_ST = '\r';
    constexpr char END_OFSTRING_ND = '\n';
    constexpr char END_OFSTRING_TRD = '\0';

    constexpr int MAX_RETRY_CONNECTIONS = 3;
    /* Paths */
    const std::string CONFIG_PATH = "configs/entities/";
    const std::string COLLISION_RULES_PATH = "configs/rules/collision_rules.json";
    const std::string MAPS_PATH = "configs/map/";
    const std::string INFINITE_MAP_PATH = "configs/map/infinite.json";

    /* Collision Rules JSON Keys */
    const std::string COLLISION_SOLID_KEY = "solid";
    const std::string COLLISION_TRIGGER_KEY = "trigger";
    const std::string COLLISION_PUSH_KEY = "push";
    const std::string COLLISION_ALLOW_KEY = "allow";

    /* Parsing constants */
    const std::string SERVER_VALUE = "server";
    const std::string CLIENT_VALUE = "client";
    const std::string BOTH_VALUE = "both";

    /* Components */
    const std::string TRANSFORMCOMPONENT = "TransformComponent";
    const std::string VELOCITYCOMPONENT = "VelocityComponent";
    const std::string SPEEDCOMPONENT = "SpeedComponent";
    const std::string SPRITECOMPONENT = "SpriteComponent";
    const std::string ANIMATIONCOMPONENT = "AnimationComponent";
    const std::string SHOOTINGSTATSCOMPONENT = "ShootingStatsComponent";
    const std::string RECTANGLERENDERCOMPONENT = "RectangleRenderComponent";
    const std::string PROJECTILEPREFABCOMPONENT = "ProjectilePrefabComponent";
    const std::string TEXTCOMPONENT = "TextComponent";
    const std::string HEALTHBARCOMPONENT = "HealthBarComponent";
    const std::string SCORECOMPONENT = "ScoreComponent";
    const std::string SCOREVALUECOMPONENT = "ScoreValueComponent";
    const std::string DAMAGECOMPONENT = "DamageComponent";
    const std::string DAMAGECOOLDOWNCOMPONENT = "DamageCooldownComponent";
    const std::string HEALTHCOMPONENT = "HealthComponent";
    const std::string HITBOXRENDERCOMPONENT = "HitboxRenderComponent";
    const std::string INTERACTIONCONFIGCOMPONENT = "InteractionConfigComponent";
    const std::string SCRIPTINGCOMPONENT = "ScriptingComponent";
    const std::string SOUNDINTENTCOMPONENT = "SoundIntentComponent";
    const std::string ENTITYPARTSCOMPONENT = "EntityPartsComponent";
    const std::string LIFETIMECOMPONENT = "LifetimeComponent";
    const std::string LIFESPANCOMPONENT = "LifeSpanComponent";
    const std::string MUSICCOMPONENT = "MusicComponent";
    const std::string COLLIDERCOMPONENT = "ColliderComponent";
    const std::string PARALLAXCOMPONENT = "ParallaxComponent";
    const std::string GAMEZONECOMPONENT = "GameZoneComponent";
    const std::string CHARGEDSHOTCOMPONENT = "ChargedShotComponent";
    const std::string INVULNERABLECOMPONENT = "InvulnerableComponent";

    /* Fields */
    const std::string SCRIPT_PATH_FIELD = "scriptPath";
    const std::string ADDITIONAL_FUNCTIONS_FIELD = "additionalFunctions";
    const std::string SCORE_FIELD = "score";
    const std::string SCOREVALUE_FIELD = "scoreValue";
    const std::string DAMAGE_FIELD = "damage";
    const std::string HEALTH_FIELD = "health";
    const std::string COOLDOWN_FIELD = "cooldown";
    const std::string ACTIVE_FIELD = "active";
    const std::string TARGET_FIELD = "target";
    const std::string POSITION_FIELD = "position";
    const std::string OFFSET_FIELD = "offset";
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
    const std::string SHOTCOUNT_FIELD = "shotCount";
    const std::string ANGLEOFFSET_FIELD = "angleOffset";
    const std::string SPREADANGLE_FIELD = "spreadAngle";
    const std::string OFFSETDISTANCE_FIELD = "offsetDistance";
    const std::string DEFAULTBEHAVIOR_FIELD = "defaultBehavior";
    const std::string ZIGZAGAMPLITUDE_FIELD = "zigzagAmplitude";
    const std::string ZIGZAGFREQUENCY_FIELD = "zigzagFrequency";
    const std::string DETECTIONRANGE_FIELD = "detectionRange";
    const std::string VERTICALDEADZONE_FIELD = "verticalDeadzone";
    const std::string WIDTH_FIELD = "width";
    const std::string HEIGHT_FIELD = "height";
    const std::string COLOR_FIELD = "color";
    const std::string R_FIELD = "r";
    const std::string G_FIELD = "g";
    const std::string B_FIELD = "b";
    const std::string FORCE_TYPE = "force";
    const std::string FORCE_TYPE_FIELD = "forceType";
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
    const std::string ZONERECT_FIELD = "zoneRect";
    const std::string NAME_FIELD = "name";
    const std::string INDEX_FIELD = "index";
    const std::string SPEEDMULTIPLIER_FIELD = "speedMultiplier";
    const std::string SCALEMODE_FIELD = "scaleMode";
    const std::string SOURCESIZE_FIELD = "sourceSize";
    const std::string REPEAT_FIELD = "repeat";
    const std::string ZINDEX_FIELD = "zIndex";
    const std::string TO_FIELD = "to";
    const std::string REWIND_FIELD = "rewind";
    const std::string TEXTUREPATH_FIELD = "texturePath";
    const std::string LOOP_FIELD = "loop";
    const std::string X_FIELD = "x";
    const std::string Y_FIELD = "y";
    const std::string ANIMATIONSTATECOMPONENT = "AnimationStateComponent";
    const std::string PREFABNAME_FIELD = "prefabName";
    const std::string LIFETIME_FIELD = "lifetime";
    const std::string LIFESPAN_FIELD = "lifespan";
    const std::string TEXT_FIELD = "text";
    const std::string FONTPATH_FIELD = "fontPath";
    const std::string SOUND_FILE_FIELD = "soundFile";
    const std::string MAPPINGS_FIELD = "mappings";
    const std::string TAGS_FIELD = "tags";
    const std::string TOENTITY_FIELD = "toEntity";
    const std::string TOSELF_FIELD = "toSelf";

    const std::string MUSICFILE_FIELD = "musicFile";
    const std::string VOLUME_FIELD = "volume";
    const std::string INITIALSTATEMUSIC_FIELD = "initialState";
    const std::string PLAYING_FIELD = "PLAYING";
    const std::string PAUSED_FIELD = "PAUSED";
    const std::string STOPPED_FIELD = "STOPPED";
    const std::string CHANGING_FIELD = "CHANGING";

    const std::string MAXCHARGE_FIELD = "maxCharge";
    const std::string CHARGERELOADTIME_FIELD = "chargeReloadTime";

    const std::string STRAIGHT_LINE_VALUE = "StraightLine";
    const std::string ZIGZAG_VALUE = "Zigzag";
    const std::string VERTICAL_MIRROR_VALUE = "VerticalMirror";
    const std::string FOLLOW_RIGHT_VALUE = "FollowRight";
    const std::string SCALEMODE_FITSCREEN = "FIT_SCREEN";
    const std::string SCALEMODE_STRETCH = "STRETCH";
    const std::string SCALEMODE_MANUAL = "MANUAL";
    const std::string COLLISION_TYPE_SOLID = "Solid";
    const std::string COLLISION_TYPE_TRIGGER = "Trigger";
    const std::string COLLISION_TYPE_PUSH = "Push";
    const std::string COLLISION_TYPE_NONE = "None";

    const float MAX_HEIGHT = 1080.0f;
    const float MAX_WIDTH = 1920.0f;
    const float GAME_ZONE_BOUNDARY_THICKNESS = 100.0f;

    const float SPATIAL_GRID_CELL_SIZE = 128.0f;
    const float SPATIAL_GRID_PADDING = 200.0f;
    const float OUT_OF_BOUNDS_MARGIN = 200.0f;

    /* Map parsing constants */
    const std::string COMPONENTS_FIELD = "components";
    const std::string BACKGROUND_FIELD = "background";
    const std::string BACKGROUND_SCROLL_SPEED_FIELD = "scrollSpeed";
    const std::string MUSIC_FIELD = "music";
    const std::string POWERUPS_FIELD = "powerUps";
    const std::string OBSTACLES_FIELD = "obstacles";
    const std::string WAVES_FIELD = "waves";
    const std::string MAP_LENGTH_FIELD = "mapLength";
    const std::string POSITIONS_FIELD = "positions";
    const std::string TYPE_FIELD = "type";
    const std::string FROMX_FIELD = "fromX";
    const std::string FROMY_FIELD = "fromY";
    const std::string POSX_FIELD = "posX";
    const std::string POSY_FIELD = "posY";
    const std::string COUNT_FIELD = "count";
    const std::string GAMEXTRIGGER_FIELD = "gameXTrigger";
    const std::string GAME_ZONE_STOP_AT_X_FIELD = "gameZoneStopAtX";
    const std::string DISTRIBUTIONX_FIELD = "distributionX";
    const std::string DISTRIBUTIONY_FIELD = "distributionY";
    const std::string ENEMIES_FIELD = "enemies";
    const std::string MIN_FIELD = "min";
    const std::string MAX_FIELD = "max";
    const math::Vector2f BACKGROUND_POSITION = math::Vector2f(0.0f, 0.0f);
    const float BACKGROUND_PARALLAX_DIRECTION_X = -1.0f;
    const float BACKGROUND_PARALLAX_DIRECTION_Y = 0.0f;
    const std::string EMPTY_PREFAB = "empty";

    const std::string HORIZONTAL_LINE_TYPE = "horizontalLine";
    const std::string VERTICAL_LINE_TYPE = "verticalLine";
    const std::string UNIQUE_TYPE = "unique";
    const std::string RANDOM_TYPE = "random";
    const std::string UNIFORM_TYPE = "uniform";

    /* Animation conditions */
    const std::string VELOCITY_UP_CONDITION = "isVelocityUp";
    const std::string VELOCITY_DOWN_CONDITION = "isVelocityDown";
    const std::string ANIMATION_END_CONDITION = "onAnimationEnd";

    /* Tags */
    const std::string CONTROLLABLETAG = "ControllableTag";
    const std::string PLAYERTAG = "PlayerTag";
    const std::string MOBTAG = "MobTag";
    const std::string SHOOTERTAG = "ShooterTag";
    const std::string PLAYERPROJECTILETAG = "PlayerProjectileTag";
    const std::string ENNEMYPROJECTILETAG = "EnnemyProjectileTag";
    const std::string PROJECTILEPASSTHROUGHTAG = "ProjectilePassThroughTag";
    const std::string GAMEZONECOLLIDERTAG = "GameZoneColliderTag";
    const std::string GAME_ZONE_STOP_TAG = "GameZoneStopTag";
    const std::string OBSTACLETAG = "ObstacleTag";
    const std::string PLAYEROBSTACLETAG = "PlayerObstacleTag";

    /* Difficulty Multipliers */
    constexpr float DIFFICULTY_EASY_MULTIPLIER = 1.3f;
    constexpr float DIFFICULTY_NORMAL_MULTIPLIER = 1.0f;
    constexpr float DIFFICULTY_HARD_MULTIPLIER = 0.7f;
    const std::string CLIENTEFFECTTAG = "ClientEffectTag";
    const std::string BACKGROUNDMUSICTAG = "BackGroundMusicTag";
    const std::string POWERUP_TAG = "PowerUpTag";
    const std::string FORCE_TAG = "ForceTag";

    /* Action constants */
    const std::string DEALDEATH_ACTION = "DealDeath";
    const std::string TAKEDEATH_ACTION = "TakeDeath";
    const std::string DEALDAMAGE_ACTION = "DealDamage";
    const std::string TAKEDAMAGE_ACTION = "TakeDamage";
    const std::string ADDLIFE_ACTION = "AddLife";
    const std::string INTERACTION_CALL_SCRIPTING_ACTION = "InteractionCallScripting";

    /* Cooldown constants */
    const float TRIGGER_DAMAGE_COOLDOWN = 0.1f;

    /* Prefabs */
    const std::string PLAYER_PREFAB_NAME = "player";
    const std::string GAME_ZONE_PREFAB = "gamezone";
    const std::string SMALL_EXPLOSION = "small_explosion";
    const std::string BIG_EXPLOSION = "big_explosion";
    const std::string OBSTACLE_1 = "obstacle1";
    const std::string ENEMY_1 = "bat";
    const std::string ENEMY_2 = "canon";
    const std::string POWERUP_ADD_LIFE = "powerUpAddLife";
    const std::string POWERUP_FORCE = "force";
    const std::string POWERUP_FLYING_FORCE = "flyingForce";
    const std::string POWERUP_SPEED = "powerUpSpeed";
    const std::string POWERUP_CHARGE_TIME = "powerUpChargeTime";

    constexpr float DEFAULT_TIMER = 0.0f;

    /* Packet constants */
    constexpr std::uint8_t PACKET_NO_OP = 0x00;
    constexpr std::uint8_t PACKET_CONNECTION = 0x01;
    constexpr std::uint8_t PACKET_ACCEPT = 0x02;
    constexpr std::uint8_t PACKET_DISC = 0x03;
    constexpr std::uint8_t PACKET_EVENT = 0x04;
    constexpr std::uint8_t PACKET_END_GAME = 0x05;
    constexpr std::uint8_t PACKET_CAN_START = 0x06;
    constexpr std::uint8_t PACKET_CLIENT_READY = 0x07;
    constexpr std::uint8_t PACKET_SPAWN = 0x08;
    constexpr std::uint8_t PACKET_DEATH = 0x09;
    constexpr std::uint8_t PACKET_WHOAMI = 0x0A;
    constexpr std::uint8_t PACKET_SERVER_STATUS = 0x0B;
    constexpr std::uint8_t PACKET_REQUEST_LOBBY = 0x0C;
    constexpr std::uint8_t PACKET_SEND_LOBBY_CODE = 0x0D;
    constexpr std::uint8_t PACKET_CONNECT_TO_LOBBY = 0x0E;
    constexpr std::uint8_t PACKET_LOBBY_MASTER_REQUEST_START = 0x0F;
    constexpr std::uint8_t PACKET_LOBBY_CONNECT_VALUE = 0x10;
    constexpr std::uint8_t PACKET_LEVEL_COMPLETE = 0x11;
    constexpr std::uint8_t PACKET_NEXT_LEVEL = 0x12;
    constexpr std::uint8_t PACKET_REGISTER = 0x13;
    constexpr std::uint8_t PACKET_CONNECT_USER = 0x14;
    constexpr std::uint8_t PACKET_LOGIN = 0x15;
    constexpr std::uint8_t PACKET_GAME_STATE_BATCH = 0x16;
    constexpr std::uint8_t PACKET_GAME_STATE_BATCH_COMPRESSED = 0x17;
    constexpr std::uint8_t PACKET_GAME_STATE_COMPRESSED = 0x18;
    constexpr std::uint8_t PACKET_REQUEST_LEADERBOARD = 0x19;
    constexpr std::uint8_t PACKET_LEADERBOARD = 0x1A;
    constexpr std::uint8_t PACKET_REGISTER_FAIL = 0x1B;
    constexpr std::uint8_t PACKET_REQUEST_PROFILE = 0x1C;
    constexpr std::uint8_t PACKET_PROFILE = 0x1D;
    constexpr std::uint8_t PACKET_GAME_RULES = 0x1E;
    constexpr std::uint8_t PACKET_REQUEST_GAME_RULES_UPDATE = 0x1F;
    constexpr std::uint8_t PACKET_NEW_CHAT = 0x20;
    constexpr std::uint8_t PACKET_BROADCASTED_CHAT = 0x21;
    constexpr std::uint8_t PACKET_FORCE_LEAVE = 0x22;

    constexpr std::uint8_t MAX_INDEX_PACKET_TYPE = 35;
    const int MAX_CLIENT_PER_LOBBY = 4;

    /* Lobby connection codes */
    const std::string LOBBY_LEAVE_MARKER = "__LEAVE__";
    const std::string LOBBY_LEAVE_KEYWORD = "LEAVE";

    enum class ForceLeaveType {
        CLOSED = 0,
        KICKED = 1,
        BANNED = 2,
    };

    /* Scripting APU constant */
    const std::string INIT_FUNCTION = "init";
    const std::string UPDATE_FUNCTION = "update";
    const std::string DEATH_FUNCTION = "death";
    const std::string ONINTERACT_FUNCTION = "OnInteract";
    const std::string ACTIVATE_OR_DEACTIVATE_FORCE_FUNCTION = "ActivateOrDeactivateForce";
    const std::string ADD_FORCE_LEVEL_FUNCTION = "addForceLevel";

    /* Constants for Scripting API */
    const std::string PRINT_FUNCTION = "print";
    const std::string CREATE_MOVE_INTENT_FUNCTION = "createMoveIntent";
    const std::string GET_ENTITY_POSITION_FUNCTION = "getEntityPosition";
    const std::string GET_NEAREST_PLAYER_POSITION_FUNCTION = "getNearestPlayerPosition";
    const std::string GET_ENTITY_SPEED_FUNCTION = "getEntitySpeed";
    const std::string SET_ENTITY_SPEED_FUNCTION = "setEntitySpeed";
    const std::string GET_MAX_CHARGE_FUNCTION = "getMaxCharge";
    const std::string SET_MAX_CHARGE_FUNCTION = "setMaxCharge";
    const std::string CREATE_SHOOT_INTENT_FUNCTION = "createShootIntent";
    const std::string SET_ANIMATION_STATE_FUNCTION = "setAnimationState";
    const std::string SPAWN_ENTITY_FUNCTION = "spawnEntity";
    const std::string GET_ENTITY_ID_FUNCTION = "getEntityId";
    const std::string ADD_PART_ID_FUNCTION = "addPartId";
    const std::string SET_PARENT_ID_FUNCTION = "setParentId";
    const std::string SET_ENTITY_ROTATION_FUNCTION = "setEntityRotation";
    const std::string GET_ENTITY_PARTS_FUNCTION = "getEntityParts";
    const std::string CREATE_DEATH_INTENT_FUNCTION = "createDeathIntent";
    const std::string IS_ENTITY_ALIVE_FUNCTION = "isEntityAlive";
    const std::string GET_PARENT_ID_FUNCTION = "getParentId";
    const std::string REMOVE_PART_ID_FUNCTION = "removePartId";
    const std::string IS_ENTITY_PLAYER_FUNCTION = "isEntityPlayer";
    const std::string GET_ENTITY_SIZE_FUNCTION = "getEntitySize";
    const std::string SET_POSITION_FUNCTION = "setPosition";
    const std::string SET_PROJECTILE_PREFAB_FUNCTION = "setProjectilePrefab";
    const std::string COUNT_FORCES_BY_TYPE_FUNCTION = "countForcesByType";
    const std::string GET_FORCE_POSITION_BY_TYPE_FUNCTION = "getForcePositionByType";
    const std::string GET_GAME_ZONE_POSITION_FUNCTION = "getGameZonePosition";
    const std::string GET_GAME_ZONE_SIZE_FUNCTION = "getGameZoneSize";
    const std::string GET_GAME_ZONE_VELOCITY_FUNCTION = "getGameZoneVelocity";
    const std::string REVERSE_SHOOT_ORIENTATION_FUNCTION = "reverseShootOrientation";
    const std::string SET_GAME_ZONE_VELOCITY_FUNCTION = "setGameZoneVelocity";
    const std::string GET_ENTITY_VELOCITY_FUNCTION = "getEntityVelocity";
    const std::string SET_INVULNERABLE_FUNCTION = "setInvulnerable";
}

#endif /* !CONSTANTS_HPP_ */
