/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** ScriptingComponent
*/

#ifndef SCRIPTINGCOMPONENT_HPP_
#define SCRIPTINGCOMPONENT_HPP_

#include "../base/IComponent.hpp"
#include <string>
#include <map>
#include "../../Error/ScriptingError.hpp"
#include "../../constants.hpp"

// To suppress warnings from sol2 includes
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wfloat-equal"
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 5321)
#endif
#include <sol/sol.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

namespace ecs {

class ScriptingComponent : public IComponent {
    public:
        ScriptingComponent(
            std::string script_name = "",
            std::vector<std::string> additionalFunctions = std::vector<std::string>(),
            std::shared_ptr<sol::state> lua = nullptr,
            size_t entityId = 0
        ) : _scriptName(script_name), _additionalFunctions(additionalFunctions), _initialized(false) {
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
        sol::function getFunction(const std::string& name) const { return _functions.at(name); };
        void addFunction(const std::string& name, const sol::function& function) { _functions[name] = function; };
        void removeFunction(const std::string& name) { _functions.erase(name); };

        std::vector<std::string> getFunctionNames() const {
            std::vector<std::string> names;
            for (const auto& pair : _functions) {
                names.push_back(pair.first);
            }
            return names;
        }

        bool isInitialized() const { return _initialized; };
        void setInitialized(bool value) { _initialized = value; };

        void clearLuaReferences() {
            _functions.clear();
            _env = sol::lua_nil;
            _initialized = false;
        }

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
