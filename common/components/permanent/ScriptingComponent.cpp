/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingComponent
*/

#include "ScriptingComponent.hpp"
#include <vector>
#include <string>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "../../Parser/ComponentRegistry/ComponentRegistrar.hpp"
#include "../../constants.hpp"

void ecs::ScriptingComponent::init(sol::state& lua, size_t entityId) {
    if (_initialized) return;
    _env = lua.create_table();
    if (!_scriptName.empty()) {
        sol::load_result script = lua.load_file(_scriptName);
        if (!script.valid()) {
            sol::error err = script;
            throw err::ScriptingError("Failed to load script: " +
                std::string(err.what()), err::ScriptingError::LOAD_FAILED);
        }

        sol::environment env(lua, sol::create, lua.globals());
        sol::protected_function chunk = script.get<sol::protected_function>();
        sol::set_environment(env, chunk);
        sol::protected_function_result result = chunk();
        _env = env;
        if (!result.valid()) {
            sol::error err = result;
            throw err::ScriptingError("Failed to run script: " +
                std::string(err.what()), err::ScriptingError::RUN_FAILED);
        }
    }

    std::vector<std::string> defaults = {constants::INIT_FUNCTION, constants::UPDATE_FUNCTION,
        constants::DEATH_FUNCTION};
    defaults.insert(defaults.end(), _additionalFunctions.begin(), _additionalFunctions.end());
    for (const auto& def : defaults) {
        sol::optional<sol::protected_function> fn = _env[def];
        if (fn)
            _functions[def] = *fn;
    }
    sol::optional<sol::protected_function> initFunc = _env[constants::INIT_FUNCTION];
    if (initFunc) {
        sol::protected_function_result result = (*initFunc)(entityId);
        if (!result.valid()) {
            sol::error err = result;
            throw err::ScriptingError("Failed to run init function: " +
                std::string(err.what()), err::ScriptingError::RUN_FAILED);
        }
    }
    _initialized = true;
}

const std::string& ecs::ScriptingComponent::getScriptName() const {
    return _scriptName;
}

REGISTER_COMPONENT(
    ecs::ScriptingComponent,
    constants::SCRIPTINGCOMPONENT,
    {
        Field{constants::TARGET_FIELD, FieldType::STRING},
        Field{constants::SCRIPT_PATH_FIELD, FieldType::STRING},
        Field{constants::ADDITIONAL_FUNCTIONS_FIELD, FieldType::JSON, true,
              std::make_shared<FieldValue>(nlohmann::json::array())}
    },
    [](const std::map<std::string, std::shared_ptr<FieldValue>>& fields) {
        auto scriptPath = std::get<std::string>(*fields.at(constants::SCRIPT_PATH_FIELD));
        std::vector<std::string> additionalFunctions;
        if (fields.find(constants::ADDITIONAL_FUNCTIONS_FIELD) !=
                fields.end()) {
            auto functionsJson = std::get<nlohmann::json>(
                *fields.at(constants::ADDITIONAL_FUNCTIONS_FIELD));
            for (const auto& func : functionsJson) {
                additionalFunctions.push_back(func);
            }
        }
        return std::make_shared<ecs::ScriptingComponent>(scriptPath, additionalFunctions);
    }
)
