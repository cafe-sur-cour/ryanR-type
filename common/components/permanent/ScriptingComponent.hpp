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
#include "../../Error/ScriptingError.hpp"
#include "../../constants.hpp"

// To suppress warnings from sol2 includes
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <sol/sol.hpp>
#pragma GCC diagnostic pop

namespace ecs {

class ScriptingComponent : public AComponent {
    public:
        ScriptingComponent(std::string script_name = "", std::vector<std::string> additionalFunctions = std::vector<std::string>(), sol::state* lua = nullptr, size_t entityId = 0)
            : _scriptName(script_name), _additionalFunctions(additionalFunctions), _initialized(false) {
                if (lua != nullptr) {
                    init(*lua, entityId);
                }
            };
        ~ScriptingComponent() = default;

        void init(sol::state& lua, size_t entityId);

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
