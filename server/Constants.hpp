/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Header
*/

#ifndef SERVER_CONSTANTS_HPP_
#define SERVER_CONSTANTS_HPP_

#include "../common/constants.hpp"

namespace constants {
    /* TPS */
    constexpr long TPS = 50;
    constexpr long CD_TPS = 20;

    /* Core */
    constexpr long SERVER_THREAD_SLEEP_MS = 10;
    constexpr int SERVER_UP = 1;

    /* Server */
    constexpr uint8_t ID_SERVER = 0;
    constexpr uint8_t BITMASK_INT = 32;
    constexpr int MAX_CLIENT = 4;

    /* Packets */
    constexpr char END_OFSTRING_ST = '\r';
    constexpr char END_OFSTRING_ND = '\n';
    constexpr char END_OFSTRING_TRD = '\0';

    const std::string USERS_JSON_PATH = "saves/users.json";
    const std::string USERNAME_JSON_WARD = "username";
    const std::string PASSWORD_JSON_WARD = "password";
    const std::string GAMES_PLAYED_JSON_WARD = "games_played";
    const std::string WINS_JSON_WARD = "wins";
    const std::string HIGH_SCORE_JSON_WARD = "high_score";
    const std::string TIME_SPENT_JSON_WARD = "time_spent";
}

#endif /* !SERVER_CONSTANTS */
