/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AComponent
*/

#ifndef ACOMPONENT_HPP_
#define ACOMPONENT_HPP_

#include "IComponent.hpp"

namespace ecs {

class AComponent : public IComponent {
    public:
        AComponent();
        ~AComponent();

        ComponentState getState() const override;
        void setState(ComponentState newState) override;

    protected:
        ComponentState _state = Permanent;

    private:
};

} // namespace ecs

#endif /* !ACOMPONENT_HPP_ */
