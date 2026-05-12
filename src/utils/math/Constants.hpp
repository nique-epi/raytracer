/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Mathematical constants shared across the project
*/

#pragma once

namespace raytracer::math::constants {

/**
 * @brief Tolerance used for near-zero comparisons (e.g. degenerate
 *        intersections, zero-length vectors).
 */
constexpr double epsilon = 1e-8;

constexpr double HALF = 0.5;

constexpr double DOUBLE = 2.0;

}  // namespace raytracer::math::constants
