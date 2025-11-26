/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** AnimationComponent
*/

#ifndef ANIMATIONCOMPONENT_HPP_
#define ANIMATIONCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include "../../../types/FRect.hpp"
#include "../../../types/Chrono.hpp"

namespace ecs {

class AnimationComponent : public AComponent {
    public:
        AnimationComponent(const std::string& texturePath, float frameWidth, float frameHeight, int frameCount, float speed = 0.1f)
            : _texturePath(texturePath),
              _frameRect(0.0f, 0.0f, frameWidth, frameHeight),
              _frameCount(frameCount), _currentFrame(0), _animationSpeed(speed),
              _chrono() {
            _chrono.start();
        }

        const math::FRect& getFrameRect() const { return _frameRect; }
        void setFrameRect(const math::FRect& rect) { _frameRect = rect; }

        float getFrameWidth() const { return _frameRect.getWidth(); }
        float getFrameHeight() const { return _frameRect.getHeight(); }
        int getFrameCount() const { return _frameCount; }
        int getCurrentFrame() const { return _currentFrame; }
        void setCurrentFrame(int frame) { _currentFrame = frame; }
        float getAnimationSpeed() const { return _animationSpeed; }
        void setAnimationSpeed(float speed) { _animationSpeed = speed; }
        math::Chrono& getChrono() { return _chrono; } // to change
        const math::Chrono& getChrono() const { return _chrono; }
        bool isValid() const { return _frameCount > 0 && _frameRect.getWidth() > 0 && _frameRect.getHeight() > 0; }
        const std::string& getTexturePath() const { return _texturePath; }
    private:
        std::string _texturePath;
        math::FRect _frameRect;
        int _frameCount;
        int _currentFrame;
        float _animationSpeed;
        math::Chrono _chrono;
};

} // namespace ecs
#endif /* !ANIMATIONCOMPONENT_HPP_ */