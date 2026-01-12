/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Optimized Game state handlers using varint encoding for bandwidth reduction
*/

#ifndef GAME_STATE_HANDLERS_OPTIMIZED_HPP_
#define GAME_STATE_HANDLERS_OPTIMIZED_HPP_

#include <memory>
#include "../../libs/Packet/PacketManager.hpp"

namespace common::packet {

bool registerOptimizedGameStateHandlers(std::shared_ptr<pm::IPacketManager> packet);
bool isOptimizedHandlersAvailable();

} // namespace common::packet

#endif /* !GAME_STATE_HANDLERS_OPTIMIZED_HPP_ */
