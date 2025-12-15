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

    /* Packets */
    constexpr char END_OFSTRING_ST = '\r';
    constexpr char END_OFSTRING_ND = '\n';
    constexpr char END_OFSTRING_TRD = '\0';
}

#endif /* !SERVER_CONSTANTS */
