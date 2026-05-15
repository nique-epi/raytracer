/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingException
*/

#ifndef SHADING_SHADINGEXCEPTION_HPP_
#define SHADING_SHADINGEXCEPTION_HPP_

#include "exceptions/Exceptions.hpp"

namespace raytracer::shading {

/**
 * @brief Base exception for the shading layer.
 *
 * Thrown when a shader, context, or strategy is wired incorrectly —
 * typically a null `IShadingMode` handed to `ShadingContext`, or a
 * null `IIntegrator` handed to `RenderedShader`. Derives from the
 * project-wide `RaytracerException` so all renderer failures share a
 * single catch site at the application boundary.
 */
class ShadingException : public raytracer::core::RaytracerException {
 public:
  using raytracer::core::RaytracerException::RaytracerException;
};

}  // namespace raytracer::shading

#endif  // SHADING_SHADINGEXCEPTION_HPP_
