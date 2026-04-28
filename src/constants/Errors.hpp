/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Errors
*/

#ifndef CONSTANTS_ERRORS_HPP_
#define CONSTANTS_ERRORS_HPP_

namespace Constants::Errors {

inline constexpr const char* PLUGIN_LOAD_FAILED = "failed to load plugin";
inline constexpr const char* PLUGIN_SYMBOL_NOT_FOUND = "failed to resolve symbol";

inline constexpr const char* IMAGE_NEGATIVE_DIMENSIONS =
    "image dimensions must be non-negative";
inline constexpr const char* IMAGE_PIXEL_OUT_OF_BOUNDS =
    "image pixel out of bounds";
inline constexpr const char* IMAGE_IO_ERROR = "image I/O error";

inline constexpr const char* MATH_VECTOR_ZERO_NORMALIZE =
    "cannot normalize a zero vector";
inline constexpr const char* MATH_GAMMA_INVALID = "gamma must be > 0";

}  // namespace Constants::Errors

#endif  // CONSTANTS_ERRORS_HPP_
