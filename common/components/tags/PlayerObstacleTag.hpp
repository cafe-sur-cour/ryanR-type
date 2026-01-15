/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** PlayerObstacleTag
*/

#ifndef PLAYEROBSTACLETAG_HPP_
#define PLAYEROBSTACLETAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class PlayerObstacleTag : public IComponent {
    public:
        PlayerObstacleTag() = default;
        ~PlayerObstacleTag() = default;
};

} // namespace ecs

#endif /* !PLAYEROBSTACLETAG_HPP_ */
