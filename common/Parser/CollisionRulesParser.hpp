#ifndef COLLISION_RULES_PARSER_HPP_
#define COLLISION_RULES_PARSER_HPP_

#include <string>
#include "../CollisionRules/CollisionRulesData.hpp"

namespace ecs {

class CollisionRulesParser {
    public:
        static CollisionRulesData parseFromFile(const std::string& filePath);
        static CollisionRulesData parseFromJsonString(const std::string& jsonString);
};

}  // namespace ecs

#endif // COLLISION_RULES_PARSER_HPP_
