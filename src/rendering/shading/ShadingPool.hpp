/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ShadingPool
*/

/**
 * @file ShadingPool.hpp
 * @brief Pre-built collection of the three `IShadingMode` strategies
 *        keyed by `scene::ViewportMode`.
 *
 * Each `ViewportMode` maps to exactly one concrete shader for the whole
 * lifetime of the application: `Wireframe → WireframeShader`,
 * `MaterialPreview → MaterialPreviewShader`, `Rendered → RenderedShader`
 * (wrapping a `WhittedIntegrator`). Building those shaders once at
 * startup and looking them up by enum is cheaper, simpler and friendlier
 * to integrator-side caches than rebuilding a fresh shader on every
 * viewport toggle.
 *
 * The pool is immutable once constructed and safe to share by `const&`
 * across threads. The active strategy chosen from the pool is installed
 * on a `ShadingContext` via `ShadingContext::setStrategy` — the pool
 * itself owns nothing else than the three shared pointers.
 */

#pragma once

#include <memory>

#include "scene/World.hpp"

namespace raytracer::shading {

class IShadingMode;

class ShadingPool {
 public:
  /**
   * @brief Build a pool wrapping three pre-instantiated shaders.
   *
   * @param[in] wireframe        Shader returned for `ViewportMode::Wireframe`.
   * @param[in] materialPreview  Shader returned for
   *                             `ViewportMode::MaterialPreview`.
   * @param[in] rendered         Shader returned for `ViewportMode::Rendered`.
   * @throws ShadingException When any of the three pointers is null.
   */
  ShadingPool(std::shared_ptr<IShadingMode> wireframe,
              std::shared_ptr<IShadingMode> materialPreview,
              std::shared_ptr<IShadingMode> rendered);

  ~ShadingPool() = default;

  ShadingPool(const ShadingPool&) = delete;
  ShadingPool& operator=(const ShadingPool&) = delete;
  ShadingPool(ShadingPool&&) = delete;
  ShadingPool& operator=(ShadingPool&&) = delete;

  /**
   * @brief Look up the pre-built shader for @p mode.
   *
   * @param[in] mode Viewport mode whose shader should be returned.
   * @returns Shared pointer to the matching `IShadingMode`. Never null.
   * @throws ShadingException When @p mode is not a known enumerator.
   */
  [[nodiscard]] std::shared_ptr<IShadingMode> get(
      scene::ViewportMode mode) const;

 private:
  std::shared_ptr<IShadingMode> wireframe_;
  std::shared_ptr<IShadingMode> materialPreview_;
  std::shared_ptr<IShadingMode> rendered_;
};

}  // namespace raytracer::shading
