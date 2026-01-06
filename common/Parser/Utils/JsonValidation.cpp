/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonValidation implementation
*/

#include "JsonValidation.hpp"
#include <sstream>

namespace parser {

ValidationResult JsonValidation::hasRequiredFields(
    const nlohmann::json& json,
    const std::vector<std::string>& requiredFields,
    const std::string& contextName
) {
    ValidationResult result;
    result.valid = true;

    std::string prefix = contextName.empty() ? "" : contextName + ": ";

    for (const auto& field : requiredFields) {
        if (!json.contains(field)) {
            result.valid = false;
            result.errors.push_back(prefix + "missing required field '" + field + "'");
        }
    }

    return result;
}

bool JsonValidation::hasFieldOfType(
    const nlohmann::json& json,
    const std::string& fieldName,
    const std::string& expectedType
) {
    if (!json.contains(fieldName)) {
        return false;
    }

    const auto& value = json[fieldName];

    if (expectedType == "string") return value.is_string();
    if (expectedType == "number") return value.is_number();
    if (expectedType == "array") return value.is_array();
    if (expectedType == "object") return value.is_object();
    if (expectedType == "boolean") return value.is_boolean();
    if (expectedType == "null") return value.is_null();

    return false;
}

std::optional<nlohmann::json> JsonValidation::getNestedField(
    const nlohmann::json& json,
    const std::string& path
) {
    std::istringstream stream(path);
    std::string segment;
    nlohmann::json current = json;

    while (std::getline(stream, segment, '.')) {
        if (!current.is_object() || !current.contains(segment)) {
            return std::nullopt;
        }
        current = current[segment];
    }

    return current;
}

}  // namespace parser
