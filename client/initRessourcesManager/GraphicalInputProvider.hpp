/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#include "../../common/ECS/resourceManager/IInputProvider.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include <memory>

namespace ecs {

class GraphicalInputProvider : public IInputProvider {
    public:
        GraphicalInputProvider(std::shared_ptr<gfx::IEvent> eventSystem);
        ~GraphicalInputProvider() override = default;

        bool isKeyPressed(event_t key) override;
        float getAxisValue(event_t axis) override;
        std::pair<int, int> getMousePos() override;
        bool isMouseButtonPressed(int button) override;

    private:
        std::shared_ptr<gfx::IEvent> _eventSystem;
};

} // namespace ecs
