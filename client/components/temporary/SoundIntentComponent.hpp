/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SoundIntentComponent
*/

#ifndef SOUNDINTENTCOMPONENT_HPP_
#define SOUNDINTENTCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include <string>

namespace ecs {

class SoundIntentComponent : public IComponent {
    public:
        SoundIntentComponent(const std::string &soundPath = "", float volume = 100.0f)
            : _soundPath(soundPath), _volume(volume) {};
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
