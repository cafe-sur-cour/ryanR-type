/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingComponent
*/

#include "ScriptingComponent.hpp"
#include <vector>
#include <string>

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
        sol::function chunk = script;
        sol::unsafe_function_result result = chunk(_env);
        if (!result.valid()) {
            sol::error err = result;
            throw err::ScriptingError("Failed to run script: " +
                std::string(err.what()), err::ScriptingError::RUN_FAILED);
        }
    }

    std::vector<std::string> defaults = {constants::INIT_FUNCTION, constants::UPDATE_FUNCTION};
    defaults.insert(defaults.end(), _additionalFunctions.begin(), _additionalFunctions.end());
    for (const auto& def : defaults) {
        sol::function fn = _env[def];
        if (fn.valid())
            _functions[def] = fn;
    }
    sol::function initFunc = _env[constants::INIT_FUNCTION];
    if (initFunc.valid()) {
        sol::unsafe_function_result result = initFunc(entityId);
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
