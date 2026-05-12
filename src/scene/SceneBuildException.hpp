/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuildException
*/

#pragma once

#include "exceptions/Exceptions.hpp"

namespace raytracer::scene {

/**
 * @brief Exception thrown when a scene is semantically invalid.
 *
 * Raised during the build phase (e.g. missing camera, missing lights,
 * unknown material reference). This is distinct from loader errors:
 * the file was read and parsed successfully, but the resulting scene
 * cannot be rendered.
 *
 * Inherits from `Core::RaytracerException`, so it is caught by the
 * top-level handler in `main` (exit code 84).
 */
class SceneBuildException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::scene
