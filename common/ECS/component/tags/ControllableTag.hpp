/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ControllableTag
*/

#ifndef CONTROLLABLETAG_HPP_
#define CONTROLLABLETAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class ControllableTag : public AComponent {
    public:
        ControllableTag() = default;
        ~ControllableTag() = default;
};

} // namespace ecs

#endif /* !CONTROLLABLETAG_HPP_ */