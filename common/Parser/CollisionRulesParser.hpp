#ifndef COLLISION_RULES_PARSER_HPP_
#define COLLISION_RULES_PARSER_HPP_

#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>
#include "../CollisionRules/CollisionRulesData.hpp"

namespace ecs {

class CollisionRulesParser {
    public:
        static CollisionRulesData parseFromFile(const std::string& filePath);
        static CollisionRulesData parseFromJsonString(const std::string& jsonString);
        static CollisionRulesData parseFromJson(const nlohmann::json& json);

    private:
        static void parseRulesForType(
            const nlohmann::json& typeJson,
            std::shared_ptr<std::vector<CollisionRule>> allowRules
        );
};

}  // namespace ecs

#endif // COLLISION_RULES_PARSER_HPP_
