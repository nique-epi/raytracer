/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Errors
*/

#ifndef CONSTANTS_ERRORS_HPP_
#define CONSTANTS_ERRORS_HPP_

namespace Constants::Errors {

constexpr const char* PLUGIN_LOAD_FAILED = "failed to load plugin";
constexpr const char* PLUGIN_SYMBOL_NOT_FOUND = "failed to resolve symbol";

constexpr const char* IMAGE_NEGATIVE_DIMENSIONS =
    "image dimensions must be non-negative";
constexpr const char* IMAGE_PIXEL_OUT_OF_BOUNDS = "image pixel out of bounds";
constexpr const char* IMAGE_IO_ERROR = "image I/O error";

constexpr const char* MATH_VECTOR_ZERO_NORMALIZE =
    "cannot normalize a zero vector";
constexpr const char* MATH_GAMMA_INVALID = "gamma must be > 0";

}  // namespace Constants::Errors

#endif  // CONSTANTS_ERRORS_HPP_
