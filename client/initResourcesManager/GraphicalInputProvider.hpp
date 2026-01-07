/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#pragma once

#include "../../common/InputMapping/IInputProvider.hpp"
#include "../../common/interfaces/IEvent.hpp"
#include "../../common/InputMapping/InputMappingManager.hpp"
#include <memory>

namespace ecs {

class GraphicalInputProvider : public IInputProvider {
    public:
        GraphicalInputProvider(std::shared_ptr<gfx::IEvent> eventSystem,
            std::shared_ptr<InputMappingManager> mappingManager);
        ~GraphicalInputProvider() override = default;

        float getAxisValue(event_t axis, size_t clientID = 0) override;

        bool isActionPressed(InputAction action, size_t clientID = 0) override;
        float getActionAxis(InputAction action, size_t clientID = 0) override;
        InputMapping getInputMapping(size_t clientID = 0) const override;

        void setToggleMode(bool enabled);
        bool isToggleMode() const;

    private:
        std::shared_ptr<gfx::IEvent> _eventSystem;
        std::shared_ptr<InputMappingManager> _mappingManager;
        bool _toggleMode;
        std::map<InputAction, bool> _toggledStates;
        std::map<InputAction, bool> _lastKeyState;
        std::map<std::pair<InputAction, gfx::EventType>, bool> _keyPressedState;
        std::map<std::pair<InputAction, gfx::EventType>, bool> _toggledKeyStates;
        std::map<std::pair<InputAction, gfx::EventType>, int> _lastToggleFrame;
        int _currentFrame;
};

} // namespace ecs
