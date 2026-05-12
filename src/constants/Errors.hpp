/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Errors
*/

#ifndef CONSTANTS_ERRORS_HPP_
#define CONSTANTS_ERRORS_HPP_

namespace raytracer::constants::errors {

constexpr const char* PLUGIN_LOAD_FAILED = "failed to load plugin";
constexpr const char* PLUGIN_SYMBOL_NOT_FOUND = "failed to resolve symbol";

constexpr const char* IMAGE_NEGATIVE_DIMENSIONS =
    "image dimensions must be non-negative";
constexpr const char* IMAGE_PIXEL_OUT_OF_BOUNDS = "image pixel out of bounds";
constexpr const char* IMAGE_IO_ERROR = "image I/O error";
constexpr const char* IMAGE_CANNOT_WRITE = "cannot write image";

constexpr const char* MATH_VECTOR_ZERO_NORMALIZE =
    "cannot normalize a zero vector";
constexpr const char* MATH_GAMMA_INVALID = "gamma must be > 0";

constexpr const char* CONFIG_FIELD_MISSING = "missing required config field";
constexpr const char* CONFIG_TYPE_INVALID = "invalid type for config field";

constexpr const char* SCENE_FILE_NOT_FOUND = "scene file not found";
constexpr const char* SCENE_PARSE_ERROR = "scene parse error";
constexpr const char* SCENE_BUILD_ERROR = "scene build error";

}  // namespace raytracer::constants::errors

#endif  // CONSTANTS_ERRORS_HPP_
