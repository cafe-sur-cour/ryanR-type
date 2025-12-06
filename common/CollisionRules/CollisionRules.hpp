#ifndef COLLISIONRULES_HPP_
#define COLLISIONRULES_HPP_

#include <vector>
#include <string>
#include "CollisionRulesData.hpp"
#include "../components/permanent/ColliderComponent.hpp"

namespace ecs {

class CollisionRules {
    public:
        static const CollisionRules& getInstance();

        static void initWithData(const CollisionRulesData& data);

        bool canCollide(
            CollisionType type,
            const std::vector<std::string>& tagsA,
            const std::vector<std::string>& tagsB
        ) const;

    private:
        CollisionRules();
        ~CollisionRules() = default;
        CollisionRules(const CollisionRules&) = delete;
        CollisionRules& operator=(const CollisionRules&) = delete;

        const std::vector<CollisionRule>& getDenyRules(CollisionType type) const;
        const std::vector<CollisionRule>& getAllowRules(CollisionType type) const;

        std::vector<CollisionRule> _solidAllowRules;
        std::vector<CollisionRule> _solidDenyRules;
        std::vector<CollisionRule> _triggerAllowRules;
        std::vector<CollisionRule> _pushAllowRules;
        std::vector<CollisionRule> _pushDenyRules;

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
