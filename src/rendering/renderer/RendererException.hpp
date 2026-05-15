/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** RendererException
*/

#ifndef RENDERER_RENDEREREXCEPTION_HPP_
#define RENDERER_RENDEREREXCEPTION_HPP_

#include "exceptions/Exceptions.hpp"

namespace raytracer::core {

/**
 * @brief Base exception for the renderer layer.
 *
 * Thrown when a renderer's `RendererConfig` or `Frame` precondition is
 * violated — typically a missing shading context, scene, or camera
 * handed to `render()`. Derives from the project-wide
 * `RaytracerException` so all renderer failures share a single catch
 * site at the application boundary.
 */
class RendererException : public RaytracerException {
 public:
  using RaytracerException::RaytracerException;
};

}  // namespace raytracer::core

#endif  // RENDERER_RENDEREREXCEPTION_HPP_
