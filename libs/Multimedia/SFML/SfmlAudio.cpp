/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** SfmlAudio
*/

#include "SfmlAudio.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <map>

namespace gfx {

SfmlAudio::SfmlAudio()
    : _musicVolume(100.0f), _soundVolume(100.0f) {
    this->_soundBuffers = std::map<std::string, std::shared_ptr<sf::SoundBuffer>>();
    this->_sounds = std::map<std::string, std::shared_ptr<sf::Sound>>();
    this->_assetManager = assets::AssetManager();
}

SfmlAudio::~SfmlAudio() {
}

void SfmlAudio::playMusic(const std::string& musicPath, bool loop) {
    std::string normalizedPath = musicPath;
    if (normalizedPath.find("./assets/") == 0)
        normalizedPath = normalizedPath.substr(9);
    if (normalizedPath.find("assets/") == 0)
        normalizedPath = normalizedPath.substr(7);

    auto assetData = _assetManager.getAsset(normalizedPath);
    if (!assetData) {
        std::cerr << "Failed to load music from embedded assets: " << musicPath << std::endl;
        return;
    }

    _musicAssetData = assetData;

    if (!_music.openFromMemory(_musicAssetData->data.data(), _musicAssetData->size)) {
        std::cerr << "Failed to open music from memory: " << musicPath << std::endl;
        return;
    }
    _music.setLooping(loop);
    _music.setVolume(_musicVolume);
    _music.play();
    if (assetData != nullptr) {
        assetData.reset();
    }
}

void SfmlAudio::stopMusic() {
    if (_music.getStatus() == sf::SoundSource::Status::Stopped)
        return;
    _music.stop();
}

void SfmlAudio::pauseMusic() {
    _music.pause();
}

void SfmlAudio::resumeMusic() {
    _music.play();
}

void SfmlAudio::setMusicVolume(float volume) {
    _musicVolume = std::max(0.0f, std::min(100.0f, volume));
    _music.setVolume(_musicVolume);
}

float SfmlAudio::getMusicVolume() const {
    return _musicVolume;
}

bool SfmlAudio::isMusicPlaying() const {
    return _music.getStatus() == sf::SoundSource::Status::Playing;
}

void SfmlAudio::loadSoundBuffer(const std::string& soundPath) {
    if (_soundBuffers.find(soundPath) != _soundBuffers.end()) {
        return;
    }

    std::string normalizedPath = soundPath;
    if (normalizedPath.find("./assets/") == 0)
        normalizedPath = normalizedPath.substr(9);
    if (normalizedPath.find("assets/") == 0)
        normalizedPath = normalizedPath.substr(7);

    auto assetData = _assetManager.getAsset(normalizedPath);
    if (!assetData) {
        std::cerr << "Failed to load sound from embedded assets: " << soundPath << std::endl;
        return;
    }

    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromMemory(assetData->data.data(), assetData->size)) {
        std::cerr << "Failed to load sound from memory: " << soundPath << std::endl;
        return;
    }
    _soundBuffers[soundPath] = buffer;
    if (assetData != nullptr) {
        assetData.reset();
    }
}

void SfmlAudio::cleanupFinishedSounds() {
    for (auto it = _sounds.begin(); it != _sounds.end();) {
        if (it->second->getStatus() == sf::SoundSource::Status::Stopped) {
            it = _sounds.erase(it);
        } else {
            ++it;
        }
    }
}

void SfmlAudio::playSound(const std::string& soundPath, float volume, float pitch) {
    loadSoundBuffer(soundPath);

    if (_soundBuffers.find(soundPath) == _soundBuffers.end()) {
        return;
    }

    cleanupFinishedSounds();

    static int soundCounter = 0;
    std::string soundKey = soundPath + "_" + std::to_string(soundCounter++);

    auto sound = std::make_shared<sf::Sound>(*_soundBuffers[soundPath]);
    sound->setVolume(std::max(0.0f, std::min(100.0f, volume)) * (_soundVolume / 100.0f));
    sound->setPitch(pitch);
    sound->play();

    _sounds[soundKey] = sound;
}

void SfmlAudio::setSoundVolume(float volume) {
    _soundVolume = std::max(0.0f, std::min(100.0f, volume));

    for (auto& [key, sound] : _sounds) {
        sound->setVolume(_soundVolume);
    }
}

float SfmlAudio::getSoundVolume() const {
    return _soundVolume;
}

void SfmlAudio::stopAllSounds() {
    for (auto& [key, sound] : _sounds) {
        if (sound->getStatus() != sf::SoundSource::Status::Stopped)
            sound->stop();
    }
    _sounds.clear();
}

}
