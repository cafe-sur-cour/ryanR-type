/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** LocalPlayerTag
*/

#ifndef LOCALPLAYERTAG_HPP_
#define LOCALPLAYERTAG_HPP_

#include "../base/AComponent.hpp"

namespace ecs {

class LocalPlayerTag : public AComponent {
    public:
        LocalPlayerTag() = default;
        ~LocalPlayerTag() = default;
};

}  // namespace ecs

#endif /* !LOCALPLAYERTAG_HPP_ */
