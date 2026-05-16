/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ViewportException
*/

#pragma once

#include "exceptions/Exceptions.hpp"

namespace raytracer::interface {

/**
 * @brief Exception for SFML viewport failures.
 *
 * Thrown when the live-display window cannot be set up — typically an
 * SFML texture or render target that fails to allocate. Derives from
 * the project-wide `RaytracerException` so viewport failures share the
 * single catch site at the application boundary.
 */
class ViewportException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::interface
