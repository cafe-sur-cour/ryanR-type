/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlAudio
*/

#ifndef SFMLAUDIO_HPP_
#define SFMLAUDIO_HPP_

#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <string>
#include <map>

#include "../IAudio.hpp"
#include "../AssetManager/AssetManager.hpp"

namespace gfx
{

class SfmlAudio : public IAudio
{
    public:
        SfmlAudio();
        ~SfmlAudio() override;

        void playMusic(const std::string& musicPath, bool loop = true) override;
        void stopMusic() override;
        void pauseMusic() override;
        void resumeMusic() override;
        void setMusicVolume(float volume) override;
        float getMusicVolume() const override;
        bool isMusicPlaying() const override;

        void playSound(const std::string& soundPath, float volume = 100.0f) override;
        void setSoundVolume(float volume) override;
        float getSoundVolume() const override;
        void stopAllSounds() override;

    private:
        std::shared_ptr<assets::AssetData> _musicAssetData;
        sf::Music _music;
        std::map<std::string, std::shared_ptr<sf::SoundBuffer>> _soundBuffers;
        std::map<std::string, std::shared_ptr<sf::Sound>> _sounds;
        float _musicVolume;
        float _soundVolume;
        assets::AssetManager _assetManager;

        void loadSoundBuffer(const std::string& soundPath);
        void cleanupFinishedSounds();
};

}

#endif /* !SFMLAUDIO_HPP_ */
