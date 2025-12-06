#ifndef COLLISION_RULES_DATA_HPP_
#define COLLISION_RULES_DATA_HPP_

#include <vector>
#include <string>

namespace ecs {

struct CollisionRule {
    std::vector<std::string> groupA;
    std::vector<std::string> groupB;
};

struct CollisionRulesData {
    std::vector<CollisionRule> solidAllowRules;
    std::vector<CollisionRule> solidDenyRules;
    std::vector<CollisionRule> triggerAllowRules;
    std::vector<CollisionRule> pushAllowRules;
    std::vector<CollisionRule> pushDenyRules;
};

}  // namespace ecs

#endif // COLLISION_RULES_DATA_HPP_
