/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** gsmStates
*/

#ifndef GSMMACHINEENUM_HPP_
#define GSMMACHINEENUM_HPP_

namespace gsm {

enum GameStateType {
    BOOT,
    LOBBY,
    LOADING,
    IN_GAME,
    GAME_END,
    SHUTDOWN
};

}  // namespace gsm

#endif /* !GSMMACHINEENUM_HPP_ */