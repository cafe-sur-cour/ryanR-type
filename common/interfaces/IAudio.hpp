/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** IAudio
*/

#ifndef IAUDIO_HPP_
    #define IAUDIO_HPP_

#include <string>

namespace gfx
{

class IAudio
{
    public:
        virtual ~IAudio() = default;

        virtual void playMusic(const std::string& musicPath, bool loop = true) = 0;
        virtual void stopMusic() = 0;
        virtual void pauseMusic() = 0;
        virtual void resumeMusic() = 0;
        virtual void setMusicVolume(float volume) = 0;
        virtual float getMusicVolume() const = 0;
        virtual bool isMusicPlaying() const = 0;

        virtual void playSound(const std::string& soundPath, float volume = 100.0f, float pitch = 1.0f) = 0;
        virtual void setSoundVolume(float volume) = 0;
        virtual float getSoundVolume() const = 0;
        virtual void stopAllSounds() = 0;
};

typedef IAudio *(*createAudio_t)();

}

#endif /* !IAUDIO_HPP_ */
