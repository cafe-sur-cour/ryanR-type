/*
** EPITECH PROJECT, 2025
** R-Type
** File description:
** Main
*/

#include <vector>
#include <memory>
#include <iostream>
#include "../libs/Packet/Packet.hpp"

int main() {
    Packet packet(1);

    std::vector<uint8_t> header = packet.packHeaderPacket(1234, 1, 0x03);
    std::cout << "Packed Header: ";
    for (auto byte : header) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;

    std::vector<uint8_t> payload = {
        0x03,
        4};
    std::vector<uint8_t> body = packet.packBodyPacket(payload);
    std::cout << "Packed Body: ";
    for (auto byte : body) {
        std::cout << std::hex << static_cast<int>(byte) << " ";
    }
    std::cout << std::dec << std::endl;
    return 0;
}
