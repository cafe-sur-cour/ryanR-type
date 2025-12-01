/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** ServerInputProvider
*/

#include "../../common/ECS/resourceManager/IInputProvider.hpp"

namespace ecs {

class ServerInputProvider : public IInputProvider {
    public:
        ServerInputProvider();
        ~ServerInputProvider() override = default;

        bool isKeyPressed(event_t key) override;
        float getAxisValue(event_t axis) override;
        std::pair<int, int> getMousePos() override;
        bool isMouseButtonPressed(int button) override;
};

} // namespace ecs
