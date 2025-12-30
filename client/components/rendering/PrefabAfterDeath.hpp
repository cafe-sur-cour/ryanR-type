/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** PrefabAfterDeath
*/

#ifndef PREFABAFTERDEATH_HPP_
#define PREFABAFTERDEATH_HPP_

#include "../../../common/components/base/AComponent.hpp"
#include <string>

namespace ecs {

class PrefabAfterDeath : public AComponent {
    public:
        PrefabAfterDeath(std::string prefabName) : _prefabName(prefabName) {};
        ~PrefabAfterDeath() = default;

        const std::string& getPrefabName() const { return _prefabName; }
        void setPrefabName(const std::string& prefabName) { _prefabName = prefabName; }
    protected:
    private:
        std::string _prefabName;
};

} // namespace ecs

#endif /* !PREFABAFTERDEATH_HPP_ */
