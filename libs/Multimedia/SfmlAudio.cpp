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

namespace gfx {

SfmlAudio::SfmlAudio()
    : _musicVolume(100.0f), _soundVolume(100.0f) {
}

void SfmlAudio::playMusic(const std::string& musicPath, bool loop) {
    auto assetData = _assetManager.getAsset(musicPath);
    if (!assetData) {
        std::cerr << "Failed to load music from embedded assets: " << musicPath << std::endl;
        return;
    }

    if (!_music.openFromMemory(assetData->data.data(), assetData->size)) {
        std::cerr << "Failed to open music from memory: " << musicPath << std::endl;
        return;
    }
    _music.setLooping(loop);
    _music.setVolume(_musicVolume);
    _music.play();
}

void SfmlAudio::stopMusic() {
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

    auto assetData = _assetManager.getAsset(soundPath);
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

void SfmlAudio::playSound(const std::string& soundPath, float volume) {
    loadSoundBuffer(soundPath);

    if (_soundBuffers.find(soundPath) == _soundBuffers.end()) {
        return;
    }

    cleanupFinishedSounds();

    static int soundCounter = 0;
    std::string soundKey = soundPath + "_" + std::to_string(soundCounter++);

    auto sound = std::make_shared<sf::Sound>(*_soundBuffers[soundPath]);
    sound->setVolume(std::max(0.0f, std::min(100.0f, volume)) * (_soundVolume / 100.0f));
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
        sound->stop();
    }
    _sounds.clear();
}

}
