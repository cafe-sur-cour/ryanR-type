/*
** EPITECH PROJECT, 2026
** ryanR-type
** File description:
** Float Quantization utilities for network optimization
*/

#ifndef FLOAT_QUANTIZATION_HPP_
#define FLOAT_QUANTIZATION_HPP_

#include <cstdint>
#include <cmath>

namespace quantization {

constexpr float POSITION_PRECISION = 100.0f;    // 0.01 units precision
constexpr float VELOCITY_PRECISION = 10.0f;     // 0.1 units/s precision
constexpr float ROTATION_PRECISION = 10.0f;     // 0.1 degrees precision
constexpr float SCALE_PRECISION = 100.0f;       // 0.01 scale precision
constexpr float SIZE_PRECISION = 100.0f;        // 0.01 size precision
constexpr float HEALTH_PRECISION = 10.0f;       // 0.1 health precision
constexpr float DAMAGE_PRECISION = 10.0f;       // 0.1 damage precision
constexpr float SPEED_PRECISION = 10.0f;        // 0.1 speed precision
constexpr float TIME_PRECISION = 100.0f;        // 0.01 seconds precision
constexpr float ANGLE_PRECISION = 100.0f;       // 0.01 radians precision

inline int32_t quantizeFloat(float value, float precision) {
    return static_cast<int32_t>(std::round(value * precision));
}

inline float dequantizeFloat(int32_t quantized, float precision) {
    return static_cast<float>(quantized) / precision;
}

inline uint64_t packQuantizedFloat(float value, float precision) {
    int32_t quantized = quantizeFloat(value, precision);
    uint64_t result = 0;
    std::memcpy(&result, &quantized, sizeof(int32_t));
    return result;
}

inline float dequantizeFloat(uint64_t packed, float precision) {
    int32_t quantized;
    std::memcpy(&quantized, &packed, sizeof(int32_t));
    return dequantizeFloat(quantized, precision);
}

inline uint64_t packPosition(float value) {
    return packQuantizedFloat(value, POSITION_PRECISION);
}

inline float unpackPosition(uint64_t packed) {
    return dequantizeFloat(packed, POSITION_PRECISION);
}

inline uint64_t packVelocity(float value) {
    return packQuantizedFloat(value, VELOCITY_PRECISION);
}

inline float unpackVelocity(uint64_t packed) {
    return dequantizeFloat(packed, VELOCITY_PRECISION);
}

inline uint64_t packRotation(float value) {
    return packQuantizedFloat(value, ROTATION_PRECISION);
}

inline float unpackRotation(uint64_t packed) {
    return dequantizeFloat(packed, ROTATION_PRECISION);
}

inline uint64_t packScale(float value) {
    return packQuantizedFloat(value, SCALE_PRECISION);
}

inline float unpackScale(uint64_t packed) {
    return dequantizeFloat(packed, SCALE_PRECISION);
}

inline uint64_t packSize(float value) {
    return packQuantizedFloat(value, SIZE_PRECISION);
}

inline float unpackSize(uint64_t packed) {
    return dequantizeFloat(packed, SIZE_PRECISION);
}

inline uint64_t packHealth(float value) {
    return packQuantizedFloat(value, HEALTH_PRECISION);
}

inline float unpackHealth(uint64_t packed) {
    return dequantizeFloat(packed, HEALTH_PRECISION);
}

inline uint64_t packDamage(float value) {
    return packQuantizedFloat(value, DAMAGE_PRECISION);
}

inline float unpackDamage(uint64_t packed) {
    return dequantizeFloat(packed, DAMAGE_PRECISION);
}

inline uint64_t packSpeed(float value) {
    return packQuantizedFloat(value, SPEED_PRECISION);
}

inline float unpackSpeed(uint64_t packed) {
    return dequantizeFloat(packed, SPEED_PRECISION);
}

inline uint64_t packTime(float value) {
    return packQuantizedFloat(value, TIME_PRECISION);
}

inline float unpackTime(uint64_t packed) {
    return dequantizeFloat(packed, TIME_PRECISION);
}

inline uint64_t packAngle(float value) {
    return packQuantizedFloat(value, ANGLE_PRECISION);
}

inline float unpackAngle(uint64_t packed) {
    return dequantizeFloat(packed, ANGLE_PRECISION);
}

}  // namespace quantization

#endif /* !FLOAT_QUANTIZATION_HPP_ */
