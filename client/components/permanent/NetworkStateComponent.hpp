/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** NetworkStateComponent
*/

#ifndef NETWORKSTATECOMPONENT_HPP_
#define NETWORKSTATECOMPONENT_HPP_

#include "../../common/components/base/IComponent.hpp"
#include "../../common/types/Vector2f.hpp"
#include <chrono>

namespace ecs {

struct NetworkTransformState {
    math::Vector2f position;
    float rotation;
    math::Vector2f scale;
    std::chrono::steady_clock::time_point timestamp;

    NetworkTransformState()
        : position(0.0f, 0.0f)
        , rotation(0.0f)
        , scale(1.0f, 1.0f)
        , timestamp(std::chrono::steady_clock::now()) {}
};

class NetworkStateComponent : public IComponent {
    public:
        NetworkStateComponent()
            : _hasTransform(false)
            , _interpolationTime(0.1f) {}

        ~NetworkStateComponent() = default;

        void setCurrentTransform(const math::Vector2f& pos, float rot, const math::Vector2f& scale) {
            if (_hasTransform) {
                _previousTransform = _currentTransform;
            }
            _currentTransform.position = pos;
            _currentTransform.rotation = rot;
            _currentTransform.scale = scale;
            _currentTransform.timestamp = std::chrono::steady_clock::now();
            _hasTransform = true;
        }

        bool hasTransform() const { return _hasTransform; }

        const NetworkTransformState& getPreviousTransform() const { return _previousTransform; }
        const NetworkTransformState& getCurrentTransform() const { return _currentTransform; }

        void setInterpolationTime(float time) { _interpolationTime = time; }
        float getInterpolationTime() const { return _interpolationTime; }

    private:
        NetworkTransformState _previousTransform;
        NetworkTransformState _currentTransform;
        bool _hasTransform;
        float _interpolationTime;
};

} // namespace ecs

#endif /* !NETWORKSTATECOMPONENT_HPP_ */
