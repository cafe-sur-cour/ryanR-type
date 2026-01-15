/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** AnimationStateComponent
*/

#ifndef ANIMATIONSTATECOMPONENT_HPP_
#define ANIMATIONSTATECOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <cstddef>
#include <string>

namespace ecs {

class AnimationStateComponent : public IComponent {
    public:
        AnimationStateComponent(const std::string& initialState = "")
            : _currentState(initialState) {};
        ~AnimationStateComponent() override = default;

        void setCurrentState(const std::string& state) { _currentState = state; }
        std::string getCurrentState() const { return _currentState; }

    private:
        std::string _currentState;
};

}  // namespace ecs


#endif /* !ANIMATIONSTATECOMPONENT_HPP_ */
