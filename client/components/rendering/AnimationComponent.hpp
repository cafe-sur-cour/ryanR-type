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
    std::string conditionKey;
    bool playRewind = false;
};

class AnimationComponent : public AComponent {
    public:
        AnimationComponent()
            : _currentState(""), _timer(0.f), _isPlaying(false), _currentFrame(0), _rewindStartFrame(-1) {}

        void addState(const std::string& name, std::shared_ptr<AnimationClip> clip) {
            _states[name] = clip;
        }

        void addTransition(const std::string& from, const std::string& to,
            const std::string& conditionKey,
            bool playRewind = false) {
            _transitions.push_back({from, to, conditionKey, playRewind});
        }

        void setCurrentState(const std::string& state) {
            if (_states.find(state) != _states.end()) {
                _currentState = state;
                _timer = 0.f;
                _isPlaying = true;
                _currentFrame = 0;
                _playRewind = false;
            }
        }

        const std::string& getCurrentState() const { return _currentState; }
        float getTimer() const { return _timer; }
        void setTimer(float timer) { _timer = timer; }
        bool isPlaying() const { return _isPlaying; }
        void setPlaying(bool playing) { _isPlaying = playing; }
        bool isPlayingRewind() const { return _playRewind; }
        void setPlayingRewind(bool rewind) { _playRewind = rewind; }

        int getRewindStartFrame() const { return _rewindStartFrame; }
        void setRewindStartFrame(int frame) { _rewindStartFrame = frame; }

        std::shared_ptr<const AnimationClip> getCurrentClip() const {
            auto it = _states.find(_currentState);
            return it != _states.end() ? it->second : nullptr;
        }

        const std::vector<Transition>& getTransitions() const { return _transitions; }

        int getCurrentFrame() const { return _currentFrame; }
        void setCurrentFrame(int frame) { _currentFrame = frame; }

        const math::FRect& getFrameRect() const { return _frameRect; }
        void setFrameRect(const math::FRect& rect) { _frameRect = rect; }

        bool isValid() const { return !_states.empty() && !_currentState.empty(); }

        bool isAnimationFinished() const {
            auto clip = getCurrentClip();
            if (!clip) return true;
            if (clip->loop) return false;
            int currentFrame = static_cast<int>(_timer / clip->speed);
            if (_playRewind) {
                return currentFrame >= clip->frameCount;
            } else {
                return currentFrame >= clip->frameCount - 1;
            }
        }

    private:
        std::unordered_map<std::string, std::shared_ptr<AnimationClip>> _states;
        std::vector<Transition> _transitions;
        std::string _currentState;
        float _timer;
        bool _isPlaying;
        bool _playRewind;
        int _currentFrame;
        int _rewindStartFrame;
        math::FRect _frameRect;
};

}  // namespace ecs

#endif /* !ANIMATIONCOMPONENT_HPP_ */
