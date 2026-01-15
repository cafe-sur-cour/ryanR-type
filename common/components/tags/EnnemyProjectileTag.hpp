/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** EnnemyProjectileTag
*/

#ifndef ENNEMYPROJECTILETAG_HPP_
#define ENNEMYPROJECTILETAG_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class EnnemyProjectileTag : public IComponent {
    public:
        EnnemyProjectileTag() = default;
        ~EnnemyProjectileTag() = default;
};

} // namespace ecs

#endif /* !ENNEMYPROJECTILETAG_HPP_ */