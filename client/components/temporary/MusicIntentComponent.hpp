/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** MusicIntentComponent
*/

#ifndef MUSICINTENTCOMPONENT_HPP_
#define MUSICINTENTCOMPONENT_HPP_

#include "../../../common/components/base/IComponent.hpp"
#include <string>

namespace ecs {

typedef enum MusicAction {
    PLAY = 0,
    PAUSE = 1,
    CHANGE = 2
} MusicAction;

class MusicIntentComponent : public IComponent {
    public:
        MusicIntentComponent(MusicAction action = PLAY, const std::string &musicPath = "", float volume = 100.0f)
            : _action(action), _musicPath(musicPath), _volume(volume) {};
        ~MusicIntentComponent() = default;

        MusicAction getAction() const { return _action; };
        void setAction(MusicAction action) { _action = action; };

        std::string getMusicPath() const { return _musicPath; };
        void setMusicPath(const std::string &musicPath) { _musicPath = musicPath; };

        float getVolume() const { return _volume; };
        void setVolume(float volume) { _volume = volume; };

    private:
        MusicAction _action;
        std::string _musicPath;
        float _volume;
};

}  // namespace ecs

#endif /* !MUSICINTENTCOMPONENT_HPP_ */
