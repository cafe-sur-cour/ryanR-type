/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EnnemyProjectileTag
*/

#ifndef ENNEMYPROJECTILETAG_HPP_
#define ENNEMYPROJECTILETAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class EnnemyProjectileTag : public AComponent {
    public:
        EnnemyProjectileTag() = default;
        ~EnnemyProjectileTag() = default;
};

} // namespace ecs

#endif /* !ENNEMYPROJECTILETAG_HPP_ */