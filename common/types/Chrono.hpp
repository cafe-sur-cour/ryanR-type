/*
** EPITECH PROJECT, 2025
** ryanR-type
** File description:
** Chrono wrapper for std::chrono
*/

#ifndef CHRONO_HPP_
#define CHRONO_HPP_

#include <chrono>

namespace math {

class Chrono {
    public:
        Chrono();
        ~Chrono() = default;

        void start();
        void stop();
        void reset();
        float getElapsedSeconds() const;
        float getElapsedMilliseconds() const;
        bool isRunning() const;

    private:
        std::chrono::high_resolution_clock::time_point _startTime;
        std::chrono::high_resolution_clock::time_point _stopTime;
        bool _isRunning;
};

} // namespace math

#endif /* !CHRONO_HPP_ */
