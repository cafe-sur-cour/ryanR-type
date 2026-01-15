/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ObstacleTag
*/

#ifndef OBSTACLETAG_HPP_
#define OBSTACLETAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class ObstacleTag : public IComponent {
    public:
        ObstacleTag() = default;
        ~ObstacleTag() = default;
};

} // namespace ecs

#endif /* !OBSTACLETAG_HPP_ */