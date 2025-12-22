#ifndef COLLISIONRULES_HPP_
#define COLLISIONRULES_HPP_

#include <vector>
#include <string>
#include "CollisionRulesData.hpp"
#include "../components/permanent/ColliderComponent.hpp"

namespace ecs {

class CollisionRules {
    public:
        CollisionRules(const CollisionRulesData& data);
        ~CollisionRules() = default;

        bool canCollide(
            CollisionType type,
            const std::vector<std::string>& tagsA,
            const std::vector<std::string>& tagsB
        ) const;

    private:
        const std::vector<CollisionRule>& getAllowRules(CollisionType type) const;

        std::shared_ptr<std::vector<CollisionRule>> _solidAllowRules;
        std::shared_ptr<std::vector<CollisionRule>> _triggerAllowRules;
        std::shared_ptr<std::vector<CollisionRule>> _pushAllowRules;

        bool entityMatchesGroup(
            const std::vector<std::string>& entityTags,
            const std::vector<std::string>& group
        ) const;
        bool ruleMatches(
            const CollisionRule& rule,
            const std::vector<std::string>& tagsA,
            const std::vector<std::string>& tagsB
        ) const;
};

}  // namespace ecs

#endif // COLLISIONRULES_HPP_
