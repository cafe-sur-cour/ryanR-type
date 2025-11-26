/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/

#include <vector>
#include <memory>

#include "ServerConfig.hpp"

#ifndef UTILS_HPP_
#define UTILS_HPP_

class Utils {
    public:
        Utils();
        ~Utils();

        void helper();
        void parsCli(int ac, char **av, std::shared_ptr<rserv::ServerConfig> config);
    protected:
    private:
};

#endif /* !UTILS_HPP_ */
