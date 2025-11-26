/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Utils
*/
#include "ClientNetwork.hpp"

#ifndef UTILS_HPP_
#define UTILS_HPP_

class Utils {
    public:
        Utils();
        ~Utils();

        void helper();
        void parseCli(int ac, char **av, std::shared_ptr<ClientNetwork> clientNetwork);
    protected:
    private:
};

#endif /* !UTILS_HPP_ */
