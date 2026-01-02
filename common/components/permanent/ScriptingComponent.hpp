/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingComponent
*/

#ifndef SCRIPTINGCOMPONENT_HPP_
#define SCRIPTINGCOMPONENT_HPP_

#include "../base/AComponent.hpp"
#include <string>
#include <map>
#include <sol/sol.hpp>
#include "../../Error/ScriptingError.hpp"
#include "../../constants.hpp"

namespace ecs {

class ScriptingComponent : public AComponent {
    public:
        ScriptingComponent(std::string script_name = "", std::vector<std::string> additionalFunctions = std::vector<std::string>(), sol::state* lua = nullptr)
            : _scriptName(script_name), _additionalFunctions(additionalFunctions), _initialized(false) {
                if (lua != nullptr) {
                    init(*lua);
                }
            };
        ~ScriptingComponent() = default;

        void init(sol::state& lua) {
            if (_initialized) return;
            _env = lua.create_table();
            if (!_scriptName.empty()) {
                sol::load_result script = lua.load_file(_scriptName);
                if (!script.valid()) {
                    sol::error err = script;
                    throw err::ScriptingError("Failed to load script: " + std::string(err.what()), err::ScriptingError::LOAD_FAILED);
                }

                sol::table metatable = lua.create_table();
                metatable["__index"] = lua["_G"];
                _env[sol::metatable_key] = metatable;
                sol::function chunk = script;
                sol::unsafe_function_result result = chunk(_env);
                if (!result.valid()) {
                    sol::error err = result;
                    throw err::ScriptingError("Failed to run script: " + std::string(err.what()), err::ScriptingError::RUN_FAILED);
                }
            }

            std::vector<std::string> defaults = {constants::INIT_FUNCTION, constants::UPDATE_FUNCTION};
            defaults.insert(defaults.end(), _additionalFunctions.begin(), _additionalFunctions.end());
            for (const auto& def : defaults) {
                sol::function fn = _env[def];
                if (fn.valid())
                    _functions[def] = fn;
            }
            _initialized = true;
        }

        const std::string& getScriptName() const;

        void setEnvironment(const sol::table& table) { _env = table; };
        sol::table getEnvironment() const { return _env; };

        bool hasFunction(const std::string& name) const { return _functions.find(name) != _functions.end(); };
        void addFunction(const std::string& name, const sol::function& function) { _functions[name] = function; };
        sol::function getFunction(const std::string& name) const { return _functions.at(name); };
        std::vector<std::string> getFunctionNames() const {
            std::vector<std::string> names;
            for (const auto& pair : _functions) {
                names.push_back(pair.first);
            }
            return names;
        }
        void removeFunction(const std::string& name) { _functions.erase(name); };
        bool isInitialized() const { return _initialized; };
        void setInitialized(bool value) { _initialized = value; };
    protected:
    private:
        std::string _scriptName;
        std::vector<std::string> _additionalFunctions;
        sol::table _env;
        std::map<std::string, sol::function> _functions;
        bool _initialized = false;
};

} // namespace ecs

#endif /* !SCRIPTINGCOMPONENT_HPP_ */
