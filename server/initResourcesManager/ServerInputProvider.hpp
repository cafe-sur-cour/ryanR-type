/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#ifndef SERVERINPUTPROVIDER_HPP_
#define SERVERINPUTPROVIDER_HPP_

#include "../../common/InputMapping/IInputProvider.hpp"
#include "../../common/InputMapping/InputMapping.hpp"
#include "../../common/constants.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>
#include <set>

namespace ecs {

class ServerInputProvider : public IInputProvider {
    public:
        ServerInputProvider();
        ~ServerInputProvider() override = default;

        float getAxisValue(event_t axis, size_t clientID = 0) override;

        bool isActionPressed(InputAction action, size_t clientID = 0) override;
        float getActionAxis(InputAction action, size_t clientID = 0) override;
        InputMapping getInputMapping(size_t clientID = 0) const override;

        void setAxisValue(ecs::InputAction action, float value, size_t clientID = 0);

        void addClientInputMapping(size_t clientID, size_t identity, const InputMapping& mapping);
        void registerClient(size_t clientID);
        void registerEntityForClient(size_t entityId, size_t clientID);
        size_t getClientIdForEntity(size_t entityId) const;
        void updateInputFromEvent(size_t clientID, constants::EventType eventType, float value);
        std::vector<size_t> getConnectedClients() const;


    private:
        std::vector<std::tuple<size_t, size_t, InputMapping>> _inputMapping;
        std::map<size_t, std::map<ecs::InputAction, float>> _clientAxisValues;
        std::map<size_t, std::map<ecs::InputAction, std::chrono::steady_clock::time_point>> _clientInputTimestamps;
        std::set<size_t> _registeredClients;
        std::map<size_t, size_t> _entityToClientId;

        static constexpr std::chrono::milliseconds INPUT_TIMEOUT = std::chrono::milliseconds(200);

        using InputHandler = void (ServerInputProvider::*)(size_t, float);
        std::vector<InputHandler> _inputHandlers;

        void handleUp(size_t clientID, float value);
        void handleDown(size_t clientID, float value);
        void handleLeft(size_t clientID, float value);
        void handleRight(size_t clientID, float value);
        void handleStop(size_t clientID, float value);
        void handleShoot(size_t clientID, float value);
        void handleForce(size_t clientID, float value);
        void handleHealthCheck(size_t clientID, float value);
};

} // namespace ecs

#endif /* !SERVERINPUTPROVIDER_HPP_ */
