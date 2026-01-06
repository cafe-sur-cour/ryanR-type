/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingComponent
*/

#include "ScriptingComponent.hpp"
#include <vector>
#include <string>
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

        sol::table metatable = lua.create_table();
        metatable["__index"] = lua["_G"];
        _env[sol::metatable_key] = metatable;
        sol::protected_function chunk = script;
        sol::protected_function_result result = chunk(_env);
        if (!result.valid()) {
            sol::error err = result;
            throw err::ScriptingError("Failed to run script: " +
                std::string(err.what()), err::ScriptingError::RUN_FAILED);
        }
    }

    std::vector<std::string> defaults = {constants::INIT_FUNCTION, constants::UPDATE_FUNCTION};
    defaults.insert(defaults.end(), _additionalFunctions.begin(), _additionalFunctions.end());
    for (const auto& def : defaults) {
        auto fn_proxy = _env[def];
        if (fn_proxy.valid()) {
            sol::function fn = fn_proxy.get<sol::function>();
            _functions[def] = fn;
        }
    }
    auto init_func_proxy = _env[constants::INIT_FUNCTION];
    if (init_func_proxy.valid()) {
        sol::function initFunc = init_func_proxy.get<sol::function>();
        sol::protected_function_result result = initFunc(entityId);
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
        if (fields.find(constants::ADDITIONAL_FUNCTIONS_FIELD) != fields.end()) {
            auto functionsJson = std::get<nlohmann::json>(*fields.at(constants::ADDITIONAL_FUNCTIONS_FIELD));
            for (const auto& func : functionsJson) {
                additionalFunctions.push_back(func);
            }
        }
        return std::make_shared<ecs::ScriptingComponent>(scriptPath, additionalFunctions);
    }
)
