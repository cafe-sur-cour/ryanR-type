/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** AnimationStateComponent
*/

#ifndef ANIMATIONSTATECOMPONENT_HPP_
#define ANIMATIONSTATECOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include <cstddef>
#include <string>

namespace ecs {

class AnimationStateComponent : public AComponent {
    public:
        AnimationStateComponent(const std::string& initialState = "")
            : currentState(initialState) {};
        ~AnimationStateComponent() override = default;

        void setCurrentState(const std::string& state) {
            currentState = state;
        }
        std::string getCurrentState() const {
            return currentState;
        }
    private:
        std::string currentState;
};

}  // namespace ecs


#endif /* !ANIMATIONSTATECOMPONENT_HPP_ */
