#ifndef COLLISIONRULES_HPP_
#define COLLISIONRULES_HPP_

#include <vector>
#include <string>
#include <fstream>
#include "../components/permanent/ColliderComponent.hpp"

namespace ecs {

class CollisionRules {
    public:
        static const CollisionRules& getInstance();

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

        struct Rule {
            std::vector<std::string> groupA;
            std::vector<std::string> groupB;
        };

        void loadFromJson(const std::string& jsonString);

        std::vector<Rule> _solidAllowRules;
        std::vector<Rule> _solidDenyRules;
        std::vector<Rule> _triggerAllowRules;
        std::vector<Rule> _pushAllowRules;
        std::vector<Rule> _pushDenyRules;

        bool entityMatchesGroup(
            const std::vector<std::string>& entityTags,
            const std::vector<std::string>& group
        ) const;
        bool ruleMatches(
            const Rule& rule,
            const std::vector<std::string>& tagsA,
            const std::vector<std::string>& tagsB
        ) const;
};

} // namespace ecs

#endif /* !COLLISIONRULES_HPP_ */
