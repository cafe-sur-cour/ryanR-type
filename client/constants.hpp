/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Client Constants
*/

#ifndef CLIENT_CONSTANTS_HPP_
#define CLIENT_CONSTANTS_HPP_

#include "../common/constants.hpp"

namespace constants {
    /* Timeout */
    constexpr int NETWORK_TIMEOUT = 5;
    constexpr int CONNECTION_ATTEMPT_TIMEOUT = 10;
    constexpr float HEALTHCHECK_INTERVAL = 5.0f;
    constexpr float WHOAMI_REQUEST_INTERVAL = 0.25f;

    /* Window */
    constexpr int WINDOW_WIDTH = 1920;
    constexpr int WINDOW_HEIGHT = 1080;

    /* UI */
    constexpr float INVALID_MOUSE_POSITION = -1.0f;
    constexpr float NAVIGATION_COOLDOWN_TIME = 0.1f;
    constexpr float UI_SCALE_SMALL = 0.75f;
    constexpr float UI_SCALE_NORMAL = 1.0f;
    constexpr float UI_SCALE_LARGE = 1.25f;
    constexpr size_t BUTTON_FONT_SIZE_BASE = 24;
    constexpr double SMALL_FONT_SIZE_MULTIPLIER = 0.5;

    /* View */
    constexpr float VIEW_SMOOTHING_SPEED = 15.0f;

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2,
    };

    /* Accessibility Filters */
    const std::string FILTER_HIGH_CONTRAST_SHADER_PATH = "assets/shaders/highcontrast.frag";
    const std::string FILTER_PROTANOPIA_SHADER_PATH = "assets/shaders/protanopia.frag";
    const std::string FILTER_DEUTERANOPIA_SHADER_PATH = "assets/shaders/deuteranopia.frag";
    const std::string FILTER_TRITANOPIA_SHADER_PATH = "assets/shaders/tritanopia.frag";
    const std::string FILTER_BRIGHTNESS_SHADER_PATH = "assets/shaders/brightness.frag";
    const std::string FILTER_BRIGHTNESS_UNIFORM_NAME = "brightness";

    /* Health Bar */
    constexpr float HEALTH_BAR_OFFSET_Y = -10.0f;
    constexpr float HEALTH_BAR_HEIGHT = 5.0f;
    constexpr float HEALTH_BAR_OUTLINE_THICKNESS = 2.0f;

    /* Settings Parsing Constants */
    const std::string ACCESSIBILITY_COLOR_BLINDNESS_STATE = "colorBlindnessState";
    const std::string ACCESSIBILITY_BRIGHTNESS_VALUE = "brightnessValue";
    const std::string ACCESSIBILITY_HIGH_CONTRAST_ENABLED = "highContrastEnabled";
    const std::string SETTINGS_UI_SCALE = "uiScale";
    const std::string SETTINGS_MUSIC_VOLUME = "musicVolume";
    const std::string SETTINGS_SOUND_VOLUME = "soundVolume";
    const std::string SETTINGS_SCREEN_RESOLUTION = "screenResolution";
    const std::string SETTINGS_TARGET_FPS = "targetFPS";
    const std::string SETTINGS_RENDER_QUALITY = "renderQuality";
    const std::string SETTINGS_USERNAME = "username";
    const std::string SETTINGS_IN_GAME_METRICS_ENABLED = "inGameMetricsEnabled";
    const std::string KEYBIND_PRIMARY = "primary";
    const std::string KEYBIND_SECONDARY = "secondary";
    const std::string KEYBIND_TOGGLE_MODE = "toggle_mode";

    /* Paths */
    const std::string SAVES_DIRECTORY = "saves";
    const std::string KEYBINDS_FILE_PATH = "saves/keybinds.json";
    const std::string ACCESSIBILITY_FILE_PATH = "saves/accessibility.json";
    const std::string SETTINGS_FILE_PATH = "saves/settings.json";
    const std::string UI_BACKGROUND_EARTH_PATH = "assets/ui/background-home.png";
    const std::string UI_BACKGROUND_CHAT = "assets/sprites/menu/chatBackgorund.png";
    const std::string HOW_TO_PLAY_PATH = "assets/sprites/menu/how_to_play.png";
    const std::string LEADERBOARD_PATH = "assets/sprites/menu/leaderboard.png";
    const std::string CHAT_PATH = "assets/sprites/menu/chat.png";
    const std::string LEADERBOARD_PLACEHOLDER_PATH = "assets/sprites/menu/leaderboard-placeholder.png";

    const std::string LOADING_PREFAVB = "configs/prefab/loading_animation.json";
    const std::string VICTORY_PREFAB = "configs/prefab/vicotry.json";
    const std::string LOSE_PREFAB = "configs/prefab/youdied.json";

    const std::string MENU_MUSIC_PATH = "assets/musics/menu.wav";
    const std::string VICTORY_MUSIC_PATH = "assets/musics/victory-music.wav";
    const std::string DEATH_MUSIC_PATH = "assets/musics/playerDeath.wav";

    const std::string MAIN_FONT = "assets/fonts/cuphead_font.ttf";

    const std::string LEVEL_COMPLETE_SUB_TITLE_TEXT = "Going to the next level...";

    /* Profile Constants */
    const std::string PROFILE_TITLE_TEXT = "YOUR PROFILE";
    const std::string USERNAME_LABEL = "Username: ";
    const std::string GAMES_PLAYED_LABEL = "Games Played: ";
    const std::string WINS_LABEL = "Wins: ";
    const std::string HIGH_SCORE_LABEL = "High Score: ";
    const std::string REFRESH_PROFILE_BUTTON_TEXT = "Refresh Profile";
    const std::string BACK_BUTTON_TEXT = "Back";

    /* Chat Constants */
    const std::string CHAT_TITLE_TEXT = "CHAT ROOM";
    const std::string CHAT_NO_MESSAGES_TEXT = "No messages yet. Start the conversation!";
    const std::string CHAT_PLACEHOLDER_TEXT = "Type your message...";
    const std::string SEND_BUTTON_TEXT = "Send";
    const std::string BACK_BUTTON_TEXT_UPPER = "BACK";

    /* How To Play Constants */
    const std::string HOW_TO_PLAY_TITLE_TEXT = "HOW TO PLAY";
    const std::string CONTROLS_TITLE_TEXT = "CONTROLS";
    const std::string OBJECTIVES_TITLE_TEXT = "OBJECTIVES";
    const std::string OBJECTIVE_DESTROY_ENEMIES = "Destroy Enemy Ships";
    const std::string OBJECTIVE_SURVIVE_WAVES = "Survive the Waves";
    const std::string OBJECTIVE_COLLECT_POWERUPS = "Collect Power-ups";
    const std::string OBJECTIVE_BEAT_HIGH_SCORE = "Beat High Score";

    /* Leaderboard Constants */
    const std::string LEADERBOARD_TITLE_TEXT = "LEADERBOARD";
    const std::string LEADERBOARD_DEFAULT_NAME_PREFIX = "Player ";
    const std::string LEADERBOARD_EMPTY_NAME = "---";
    const std::string LEADERBOARD_DEFAULT_SCORE = "0";

    /* Home page input place holders */
    const std::string IP_PLACEHOLDER = "Enter an IP address";
    const std::string PORT_PLACEHOLDER = "Enter a port";

     /* Replay Constants */
     constexpr int MAX_REPLAY_FILES = 5;
    const std::string REPLAY_DIRECTORY = "saves/replays";
    const std::string REPLAY_FILE_PREFIX = "replay";
    const std::string REPLAY_BUTTON_TEXT = "Replay";
    const std::string REPLAY_TOTAL_TIME = "totalTime";
    const std::string REPLAY_RENDERABLES = "renderables";
    const std::string REPLAY_GAMEZONE = "gamezone";
    const std::string REPLAY_AUDIO = "audio";
    const std::string REPLAY_TYPE = "type";
    const std::string REPLAY_TRANSFORM = "transform";
    const std::string REPLAY_X = "x";
    const std::string REPLAY_Y = "y";
    const std::string REPLAY_WIDTH = "width";
    const std::string REPLAY_HEIGHT = "height";
    const std::string REPLAY_ROTATION = "r";
    const std::string REPLAY_SCALE_X = "sx";
    const std::string REPLAY_SCALE_Y = "sy";
    const std::string REPLAY_COLOR = "color";
    const std::string REPLAY_RED = "r";
    const std::string REPLAY_GREEN = "g";
    const std::string REPLAY_BLUE = "b";
    const std::string REPLAY_ALPHA = "a";
    const std::string REPLAY_SPRITE = "sprite";
    const std::string REPLAY_TEXTURE = "texture";
    const std::string REPLAY_OFFSET_X = "offsetX";
    const std::string REPLAY_OFFSET_Y = "offsetY";
    const std::string REPLAY_PARALLAX = "parallax";
    const std::string REPLAY_BASE_SCROLL_SPEED = "baseScrollSpeed";
    const std::string REPLAY_DIRECTION = "direction";
    const std::string REPLAY_LAYERS = "layers";
    const std::string REPLAY_NAME = "name";
    const std::string REPLAY_FILE_PATH = "filePath";
    const std::string REPLAY_SPEED_MULTIPLIER = "speedMultiplier";
    const std::string REPLAY_SCALE = "scale";
    const std::string REPLAY_SCALE_MODE = "scaleMode";
    const std::string REPLAY_SOURCE_SIZE = "sourceSize";
    const std::string REPLAY_REPEAT = "repeat";
    const std::string REPLAY_Z_INDEX = "zIndex";
    const std::string REPLAY_CURRENT_OFFSET = "currentOffset";
    const std::string REPLAY_HEALTH = "health";
    const std::string REPLAY_CURRENT = "current";
    const std::string REPLAY_MAX = "max";
    const std::string REPLAY_COLLIDER = "collider";
    const std::string REPLAY_SIZE_X = "sizeX";
    const std::string REPLAY_SIZE_Y = "sizeY";
    const std::string REPLAY_TEXT = "text";
    const std::string REPLAY_CONTENT = "content";
    const std::string REPLAY_FONT_PATH = "fontPath";
    const std::string REPLAY_RECTANGLE = "rectangle";
    const std::string REPLAY_HITBOX = "hitbox";
    const std::string REPLAY_SOUND_PATH = "soundPath";
    const std::string REPLAY_VOLUME = "volume";
    const std::string REPLAY_TYPE_PARALLAX = "parallax";
    const std::string REPLAY_TYPE_HEALTHBAR = "healthbar";
    const std::string REPLAY_TYPE_TEXT = "text";
    const std::string REPLAY_TYPE_RECTANGLE = "rectangle";
    const std::string REPLAY_TYPE_HITBOX = "hitbox";
    const std::string REPLAY_TYPE_SOUND = "sound";

    /* Level Constants */
    const size_t MAX_HISTORY_SIZE = 50;
    const float CHANGE_DEBOUNCE_TIME = 1.0f;
    const float CHANGE_DEBOUNCE_TIME_SHORT = 0.25f;

    const std::string LEVEL_FILE_PREFIX = "level";
    const std::string LEVEL_FILE_EXTENSION = ".json";
    const std::string LEVEL_DIRECTORY = "configs/map";
    const std::string MUSIC_DIRECTORY = "configs/entities/musics";
    const std::string BACKGROUNDS_DIRECTORY = "configs/entities/backgrounds";
    const std::string OBSTACLES_DIRECTORY = "configs/entities/obstacles";
    const std::string POWERUPS_DIRECTORY = "configs/entities/powerUp";
    const std::string ENEMIES_DIRECTORY = "configs/entities/enemies";

    /* Splash Screen Constants */
    const float SPLASHSCREEN_GROW_DURATION = 1.0f;
    const float SPLASHSCREEN_TOTAL_DURATION = 5.0f;
    const float SPLASHSCREEN_START_FONT_SIZE = 10.0f;
    const float SPLASHSCREEN_TARGET_FONT_SIZE = 150.0f;
    const std::string SPLASH_MAIN_TITLE_TEXT = "R-Cup";
    const std::string SPLASH_SUBTITLE_TEXT = "By RyanR-Type";
    constexpr float SPLASH_TITLE_OUTLINE_THICKNESS = 5.0f;
    constexpr float SPLASH_SUBTITLE_OUTLINE_THICKNESS = 2.0f;
    constexpr float SPLASH_TEXT_PADDING = 20.0f;
}

#endif /* !CLIENT_CONSTANTS_HPP_ */
