/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Chrono implementation
*/

#include "Chrono.hpp"

namespace math {

Chrono::Chrono()
    : _isRunning(false) {
    _startTime = std::chrono::high_resolution_clock::now();
    _stopTime = _startTime;
}

void Chrono::start() {
    if (!_isRunning) {
        _startTime = std::chrono::high_resolution_clock::now();
        _isRunning = true;
    }
}

void Chrono::stop() {
    if (_isRunning) {
        _stopTime = std::chrono::high_resolution_clock::now();
        _isRunning = false;
    }
}

void Chrono::reset() {
    _startTime = std::chrono::high_resolution_clock::now();
    _stopTime = _startTime;
    _isRunning = false;
}

float Chrono::getElapsedSeconds() const {
    auto endPoint = _isRunning ?
        std::chrono::high_resolution_clock::now() : _stopTime;
    auto duration = std::chrono::duration_cast<
        std::chrono::duration<float>>(endPoint - _startTime);
    return duration.count();
}

float Chrono::getElapsedMilliseconds() const {
    auto endPoint = _isRunning ?
        std::chrono::high_resolution_clock::now() : _stopTime;
    auto duration = std::chrono::duration_cast<
        std::chrono::milliseconds>(endPoint - _startTime);
    return static_cast<float>(duration.count());
}

bool Chrono::isRunning() const {
    return _isRunning;
}

}  // namespace math
