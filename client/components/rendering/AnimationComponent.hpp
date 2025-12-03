/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationComponent
*/

#ifndef ANIMATIONCOMPONENT_HPP_
#define ANIMATIONCOMPONENT_HPP_

#include <unordered_map>
#include <vector>
#include <functional>
#include <string>
#include "../../../common/components/base/AComponent.hpp"
#include "../../../common/types/FRect.hpp"
#include "../../../common/ECS/entity/Entity.hpp"

namespace ecs {

class Registry;

struct AnimationClip {
    std::string texturePath;
    float frameWidth;
    float frameHeight;
    int frameCount;
    float startWidth;
    float startHeight;
    float speed;
    bool loop;
};

struct Transition {
    std::string from;
    std::string to;
    std::function<bool(std::shared_ptr<Registry>, Entity)> condition;
};

class AnimationComponent : public AComponent {
    public:
        AnimationComponent()
            : _currentState(""), _timer(0.f), _isPlaying(false), _currentFrame(0) {}

        void addState(const std::string& name, const AnimationClip& clip) {
            _states[name] = clip;
        }

        void addTransition(const std::string& from, const std::string& to,
            std::function<bool(std::shared_ptr<Registry>, Entity)> condition) {
            _transitions.push_back({from, to, condition});
        }

        void setCurrentState(const std::string& state) {
            if (_states.find(state) != _states.end()) {
                _currentState = state;
                _timer = 0.f;
                _isPlaying = true;
                _currentFrame = 0;
            }
        }

        const std::string& getCurrentState() const { return _currentState; }
        float getTimer() const { return _timer; }
        void setTimer(float timer) { _timer = timer; }
        bool isPlaying() const { return _isPlaying; }
        void setPlaying(bool playing) { _isPlaying = playing; }

        const AnimationClip* getCurrentClip() const {
            auto it = _states.find(_currentState);
            return it != _states.end() ? &it->second : nullptr;
        }

        const std::vector<Transition>& getTransitions() const { return _transitions; }

        int getCurrentFrame() const { return _currentFrame; }
        void setCurrentFrame(int frame) { _currentFrame = frame; }

        const math::FRect& getFrameRect() const { return _frameRect; }
        void setFrameRect(const math::FRect& rect) { _frameRect = rect; }

        bool isValid() const { return !_states.empty() && !_currentState.empty(); }

    private:
        std::unordered_map<std::string, AnimationClip> _states;
        std::vector<Transition> _transitions;
        std::string _currentState;
        float _timer;
        bool _isPlaying;
        int _currentFrame;
        math::FRect _frameRect;
};

}  // namespace ecs

#endif /* !ANIMATIONCOMPONENT_HPP_ */
