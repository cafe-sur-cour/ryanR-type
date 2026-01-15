/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicComponent
*/

#ifndef MUSICCOMPONENT_HPP_
#define MUSICCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include <string>


namespace ecs {

typedef enum MusicState {
    PLAYING = 0,
    PAUSED = 1,
    CHANGING = 2,
    STOPPED = 3
} MusicState;

class MusicComponent : public IComponent {
    public:
        MusicComponent(
            std::string musicFile = "", MusicState initialState = STOPPED, float volume = 100.0f, bool loop = false
        ) : _currentMusic(musicFile), _state(initialState), _volume(volume), _loop(loop) {};
        ~MusicComponent() = default;

        void playMusic() { _state = PLAYING; };
        void pauseMusic() { _state = PAUSED; };
        void stopMusic() { _state = STOPPED; };
        bool isPlaying() const { return _state == PLAYING; };
        MusicState getState() const { return _state; };
        void playNewMusic(const std::string& musicFile) {
            _currentMusic = musicFile;
            _state = CHANGING;
        };

        std::string getCurrentMusic() const { return _currentMusic; };
        void setCurrentMusic(const std::string& musicFile) { _currentMusic = musicFile; };

        float getVolume() const { return _volume; };
        void setVolume(float volume) { _volume = volume; };

        bool isLooping() const { return _loop; };
        void setLoop(bool loop) { _loop = loop; };

    protected:
    private:
        std::string _currentMusic;
        MusicState _state;
        float _volume;
        bool _loop;
};

}  // namespace ecs

#endif /* !MUSICCOMPONENT_HPP_ */
