/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** JsonValidation
*/

#ifndef JSONVALIDATION_HPP_
#define JSONVALIDATION_HPP_

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>

namespace parser {

struct ValidationResult {
    bool valid;
    std::vector<std::string> errors;
    std::vector<std::string> warnings;

    operator bool() const { return valid; }
};

class JsonValidation {
public:
    static ValidationResult hasRequiredFields(
        const nlohmann::json& json,
        const std::vector<std::string>& requiredFields,
        const std::string& contextName = ""
    );

    static bool hasFieldOfType(
        const nlohmann::json& json,
        const std::string& fieldName,
        const std::string& expectedType
    );

    template<typename T>
    static T getOrDefault(
        const nlohmann::json& json,
        const std::string& fieldName,
        const T& defaultValue
    ) {
        if (json.contains(fieldName)) {
            try {
                return json[fieldName].get<T>();
            } catch (...) {
                return defaultValue;
            }
        }
        return defaultValue;
    }

    static std::optional<nlohmann::json> getNestedField(
        const nlohmann::json& json,
        const std::string& path
    );

private:
    JsonValidation() = delete;
};

}  // namespace parser

#endif /* !JSONVALIDATION_HPP_ */
