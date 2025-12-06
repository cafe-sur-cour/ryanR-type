#ifndef COLLISION_RULES_DATA_HPP_
#define COLLISION_RULES_DATA_HPP_

#include <vector>
#include <string>
#include <memory>

namespace ecs {

struct CollisionRule {
    std::vector<std::string> groupA;
    std::vector<std::string> groupB;
};

struct CollisionRulesData {
    std::shared_ptr<std::vector<CollisionRule>> solidAllowRules;
    std::shared_ptr<std::vector<CollisionRule>> solidDenyRules;
    std::shared_ptr<std::vector<CollisionRule>> triggerAllowRules;
    std::shared_ptr<std::vector<CollisionRule>> triggerDenyRules;
    std::shared_ptr<std::vector<CollisionRule>> pushAllowRules;
    std::shared_ptr<std::vector<CollisionRule>> pushDenyRules;
};

}  // namespace ecs

#endif // COLLISION_RULES_DATA_HPP_
