/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** InvulnerableComponent
*/

#ifndef INVULNERABLECOMPONENT_HPP_
#define INVULNERABLECOMPONENT_HPP_

#include "../base/IComponent.hpp"

namespace ecs {

class InvulnerableComponent : public IComponent {
    public:
        InvulnerableComponent(bool active = false) : _active(active) {};
        ~InvulnerableComponent()  = default;

        bool isActive() const { return _active; };
        void setActive(bool active) { _active = active; };

    private:
        bool _active;
};

}  // namespace ecs

#endif /* !INVULNERABLECOMPONENT_HPP_ */
