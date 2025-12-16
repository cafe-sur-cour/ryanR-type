/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Game state pack/unpack handlers (common)
*/

#ifndef COMMON_GAME_STATE_HANDLERS_HPP_
#define COMMON_GAME_STATE_HANDLERS_HPP_

#include <memory>
#include "DefaultPacketHandlers.hpp"

namespace common::packet {
    bool registerGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet);
}

#endif // COMMON_GAME_STATE_HANDLERS_HPP_
