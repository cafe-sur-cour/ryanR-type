/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** BackGroundMusicTag
*/

#ifndef BACKGROUNDMUSICTAG_HPP_
#define BACKGROUNDMUSICTAG_HPP_

#include "../../../common/components/base/IComponent.hpp"

namespace ecs {

class BackGroundMusicTag : public IComponent {
    public:
        BackGroundMusicTag() = default;
        ~BackGroundMusicTag() = default;
};

} // namespace ecs

#endif /* !BACKGROUNDMUSICTAG_HPP_ */
