/*
** EPITECH PROJECT, 2025
** r-type
** File description:
** SystemLoader
*/

#ifndef SYSTEMLOADER_HPP_
#define SYSTEMLOADER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include "../DLLoader/ILoader.hpp"
#include "base/ISystem.hpp"

namespace ecs {

typedef ISystem* (*createSystem_t)();
typedef const char* (*getSystemName_t)();

class SystemLoader {
    public:
        SystemLoader();
        ~SystemLoader();

        std::shared_ptr<ISystem> loadSystem(const std::string& name);

    private:
        std::unordered_map<std::string, std::unique_ptr<ILoader>> _loaders;
};

}  // namespace ecs

#endif /* !SYSTEMLOADER_HPP_ */
