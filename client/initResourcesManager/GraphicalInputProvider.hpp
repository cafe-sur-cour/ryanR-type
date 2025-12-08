/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** GraphicalInputProvider
*/

#include "../../common/InputMapping/IInputProvider.hpp"
#include "../../libs/Multimedia/IEvent.hpp"
#include "../../common/InputMapping/InputMapping.hpp"
#include <memory>

namespace ecs {

class GraphicalInputProvider : public IInputProvider {
    public:
        GraphicalInputProvider(std::shared_ptr<gfx::IEvent> eventSystem, const InputMapping& mapping);
        ~GraphicalInputProvider() override = default;

        float getAxisValue(event_t axis) override;

        bool isActionPressed(InputAction action) override;
        float getActionAxis(InputAction action) override;
        InputMapping getInputMapping() const override;

    private:
        std::shared_ptr<gfx::IEvent> _eventSystem;
        InputMapping _mapping;
};

} // namespace ecs
