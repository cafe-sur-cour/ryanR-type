/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** DeletionIntentComponent
*/

#ifndef DELETIONINTENTCOMPONENT_HPP_
#define DELETIONINTENTCOMPONENT_HPP_

#include "../../common/components/base/AComponent.hpp"
#include <vector>
#include <string>

namespace ecs {

class DeletionIntentComponent : public AComponent {
    public:
        DeletionIntentComponent() = default;
        ~DeletionIntentComponent() = default;

        void addTag(const std::string& tagName) {
            _tags.push_back(tagName);
        }

        const std::vector<std::string>& getTags() const {
            return _tags;
        }

    private:
        std::vector<std::string> _tags;
};

}  // namespace ecs

#endif /* !DELETIONINTENTCOMPONENT_HPP_ */