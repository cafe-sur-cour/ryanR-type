/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundIntentComponent
*/

#ifndef SOUNDINTENTCOMPONENT_HPP_
#define SOUNDINTENTCOMPONENT_HPP_

#include "../../../common/components/base/AComponent.hpp"
#include <string>

namespace ecs {

class SoundIntentComponent : public AComponent {
    public:
        SoundIntentComponent(const std::string &soundPath = "", float volume = 100.0f)
            : _soundPath(soundPath), _volume(volume) {
            _state = Temporary;
        };
        ~SoundIntentComponent() = default;

        std::string getSoundPath() const { return _soundPath; };
        void setSoundPath(const std::string &soundPath) { _soundPath = soundPath; };

        float getVolume() const { return _volume; };
        void setVolume(float volume) { _volume = volume; };

    private:
        std::string _soundPath;
        float _volume;
};

}  // namespace ecs

#endif /* !SOUNDINTENTCOMPONENT_HPP_ */
