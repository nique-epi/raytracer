/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WhittedIntegrator
*/

/**
 * @file WhittedIntegrator.hpp
 * @brief Whitted-style ray-tracing integrator.
 *
 * Solves the rendering equation for a primary ray using the classic
 * Whitted-1980 recipe: direct lighting from every `ILight` (Lambert
 * cosine + shadow ray + inverse-square falloff), plus an indirect
 * bounce obtained by sampling the material's scatter ray and recursing
 * up to `depth` levels.
 *
 * Scope: this class only implements the "Rendered" viewport-mode
 * pipeline. Viewport-mode dispatching (Wireframe / MaterialPreview /
 * Rendered) lives in the `shading::` layer; the integrator is wrapped
 * by `RenderedShader` and never sees the other modes.
 *
 * Background policy: the integrator samples the scene background for
 * the primary ray only. Secondary scatter rays that escape the scene
 * return black (no environment lighting), matching the Rendered mode
 * contract documented on `ViewportMode::Rendered`.
 *
 * Ambient occlusion: applied as a multiplicative factor on the radiance
 * carried by primary rays only (per `AmbientOcclusionSettings`). The
 * factor is computed by cosine-weighted hemisphere sampling around the
 * shading normal; secondary bounces are never multiplied by AO.
 */

#pragma once

#include "World.hpp"
#include "common/helper/Logger.hpp"
#include "rendering/integrator/IIntegrator.hpp"

namespace raytracer::core {

class WhittedIntegrator : public IIntegrator {
 public:
  WhittedIntegrator() = default;
  ~WhittedIntegrator() override = default;

  WhittedIntegrator(const WhittedIntegrator&) = delete;
  WhittedIntegrator& operator=(const WhittedIntegrator&) = delete;
  WhittedIntegrator(WhittedIntegrator&&) = delete;
  WhittedIntegrator& operator=(WhittedIntegrator&&) = delete;

  math::Color computeRadiance(const math::Ray& ray, const scene::Scene& scene,
                              int depth) override;

 private:
  /**
   * @brief Recursive Whitted shading core: intersect, shade, optionally
   *        recurse on the material scatter ray.
   *
   * AO is applied here (only when @p isPrimary is true) so secondary
   * bounces stay un-occluded.
   *
   * @param[in] ray       Incoming ray to trace.
   * @param[in] scene     Scene queried for intersections, lights and
   *                      background.
   * @param[in] depth     Remaining bounce budget. The recursion stops
   *                      and returns black when this hits 0.
   * @param[in] isPrimary True when @p ray is a camera ray, false for
   *                      any scatter ray spawned by the recursion.
   * @returns Linear-space radiance carried back along @p ray.
   */
  math::Color castRay(const math::Ray& ray, const scene::Scene& scene,
                      int depth, bool isPrimary);

  /**
   * @brief Compute the AO multiplicative factor at a shaded point.
   *
   * Casts @p settings.samples cosine-weighted shadow rays in the
   * hemisphere around @p unitNormal and returns
   * `1 - intensity * (blocked / samples)`.
   *
   * @param[in] shadingPoint Surface point already offset along the
   *                         normal to avoid self-occlusion.
   * @param[in] unitNormal   Unit surface normal at @p shadingPoint.
   * @param[in] scene        Scene queried for occlusion intersections.
   * @param[in] settings     AO configuration (samples, radius,
   *                         intensity, enabled flag).
   * @returns AO factor in [0, 1]. Returns 1.0 when AO is disabled or
   *          when @p settings.samples is non-positive.
   */
  double computeAmbientOcclusion(
      const math::Vector3D& shadingPoint, const math::Vector3D& unitNormal,
      const scene::Scene& scene,
      const scene::AmbientOcclusionSettings& settings);

  raytracer::common::Logger logger_{"WhittedIntegrator"};
};

}  // namespace raytracer::core
