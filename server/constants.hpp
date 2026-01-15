/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_CONSTANTS_HPP_
#define SERVER_CONSTANTS_HPP_

#include <cstdint>
#include "../common/constants.hpp"

namespace constants {
    /* TPS */
    constexpr int64_t TPS = 60;

    /* Core */
    constexpr long SERVER_THREAD_SLEEP_MS = 10;
    constexpr int SERVER_UP = 1;

    /* Server */
    constexpr uint8_t ID_SERVER = 0;
    constexpr uint8_t BITMASK_INT = 32;
    constexpr int MAX_CLIENT = 4;
    constexpr int CLIENT_TIMEOUT_SECONDS = 15;

    /* HTTP Server */
    const std::string DEFAULT_HTTP_PASSWORD = "defaultpassword";
    const std::string HTTP_ENV_FILE_PATH = "./server/http/.env";
    constexpr int HTTP_SERVER_PORT = 5173;

    constexpr float SHOOT_INPUT_COOLDOWN = 0.2f;

    const std::string USERS_JSON_PATH = "saves/users.json";
    const std::string SCORES_JSON_PATH = "saves/scores.json";
    const std::string USERNAME_JSON_WARD = "username";
    const std::string PASSWORD_JSON_WARD = "password";
    const std::string GAMES_PLAYED_JSON_WARD = "games_played";
    const std::string WINS_JSON_WARD = "wins";
    const std::string HIGH_SCORE_JSON_WARD = "high_score";
    const std::string TIME_SPENT_JSON_WARD = "time_spent";
    const std::string BANNED_JSON_WARD = "banned";
    const std::string SCORE_JSON_WARD = "scores";
    const std::string GODMOD_JSON_WARD = "godmod";
}

#endif /* !SERVER_CONSTANTS */
