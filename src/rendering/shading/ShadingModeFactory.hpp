/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingModeFactory
*/

/**
 * @file ShadingModeFactory.hpp
 * @brief Maps a `scene::ViewportMode` to its concrete `IShadingMode`.
 *
 * The factory is the single place that knows how to build each shader
 * (and, for `Rendered`, which `IIntegrator` to wrap). Both the startup
 * path (`Application::createShadingContext`) and the interactive
 * viewport (live mode switching) go through it, so the mode-to-shader
 * mapping never drifts between the two.
 */

#pragma once

#include <memory>

#include "scene/World.hpp"

namespace raytracer::shading {

class IShadingMode;

class ShadingModeFactory {
 public:
  ShadingModeFactory() = delete;

  /**
   * @brief Build the shader implementing @p mode.
   *
   * @param[in] mode Viewport shading mode to instantiate.
   * @returns Owning pointer to the matching `IShadingMode`.
   * @throws ShadingException When @p mode is not a known enumerator.
   */
  static std::shared_ptr<IShadingMode> create(scene::ViewportMode mode);
};

}  // namespace raytracer::shading
