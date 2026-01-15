/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DamageIntentComponent
*/

#ifndef DAMAGEINTENTCOMPONENT_HPP_
#define DAMAGEINTENTCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include "../../ECS/entity/Entity.hpp"

namespace ecs {

class DamageIntentComponent : public IComponent {
    public:
        DamageIntentComponent(float damages = 0.0f, ecs::Entity source = 0) : _damages(damages), _source(source) {};
        ~DamageIntentComponent() = default;

        float getDamages() { return _damages; };
        void setDamages(float damages) { _damages = damages; };

        ecs::Entity getSource() const { return _source; };
        void setSource(ecs::Entity source) { _source = source; };

    private:
        float _damages;
        ecs::Entity _source;
};

}  // namespace ecs

#endif /* !DAMAGEINTENTCOMPONENT_HPP_ */
