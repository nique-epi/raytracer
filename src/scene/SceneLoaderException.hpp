/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneLoaderException
*/

#pragma once

#include "exceptions/Exceptions.hpp"

namespace raytracer::scene {

/**
 * @brief Intermediate base for all scene-file loading errors.
 *
 * Callers that want to catch any I/O or parse problem in one place
 * can catch `SceneLoaderException`. More specific handlers can catch
 * `SceneFileNotFoundException` or `SceneParseException` directly.
 *
 * Inherits from `Core::RaytracerException`, so it is also caught by the
 * top-level handler in `main` (exit code 84).
 */
class SceneLoaderException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::scene
