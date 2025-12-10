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
        void updateInputFromEvent(size_t clientID, constants::EventType eventType, float value);
        std::vector<size_t> getConnectedClients() const;


    private:
        std::vector<std::tuple<size_t, size_t, InputMapping>> _inputMapping;
        std::map<size_t, std::map<ecs::InputAction, float>> _clientAxisValues;
};

} // namespace ecs

#endif /* !SERVERINPUTPROVIDER_HPP_ */
