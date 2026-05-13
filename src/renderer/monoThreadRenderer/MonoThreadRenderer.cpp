/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MonoThreadRenderer
*/

#include "MonoThreadRenderer.hpp"
#include <algorithm>
#include <limits>
#include <random>
#include <utility>
#include "components/camera/ICamera.hpp"
#include "components/light/ILight.hpp"
#include "components/material/IMaterial.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

constexpr double primaryRayTMin = 0.001;
constexpr double normalDebugHalf = 0.5;

/**
 * @brief Return a unit-length shading normal for @p rec.
 *
 * `HitRecord::normal` is not guaranteed to be unit length: transformed
 * primitives apply a transform without renormalising. Cosine factors
 * computed with a non-unit normal would scale unpredictably and the
 * shadow-ray offset along the normal could be far from one epsilon.
 * Normalising here once per shading point fixes both issues. The
 * length-zero guard falls back to the raw normal to avoid producing
 * a NaN vector — a degenerate hit will simply read as black further
 * down the chain.
 *
 * @param [in] rec Hit record from the intersection test.
 * @returns Unit-length normal, or the raw normal when its length is zero.
 */
raytracer::math::Vector3D unitShadingNormal(
    const raytracer::math::HitRecord& rec) {
  const double lengthSquared = rec.normal.lengthSquared();
  if (lengthSquared <= 0.0) {
    return rec.normal;
  }
  return rec.normal / std::sqrt(lengthSquared);
}

/**
 * @brief Map a unit normal to an RGB colour for the Wireframe preview.
 *
 * Components are remapped from [-1, 1] to [0, 1] so the colour stays in
 * the displayable range without losing the orientation signal.
 */
raytracer::math::Color normalAsColor(const raytracer::math::Vector3D& normal) {
  return {(normal.x * normalDebugHalf) + normalDebugHalf,
          (normal.y * normalDebugHalf) + normalDebugHalf,
          (normal.z * normalDebugHalf) + normalDebugHalf};
}

/**
 * @brief Lambertian direct-lighting contribution from a single light.
 *
 * Cheap geometric work first (light direction, cosine), then the
 * potentially expensive `illuminate()` which fires a shadow ray. This
 * ordering means backfacing surfaces and zero-albedo materials never
 * pay for the shadow-ray cost.
 *
 * @param [in] light           Light source to evaluate.
 * @param [in] shadingPoint    Surface point already offset along the
 *                             normal to avoid self-shadowing.
 * @param [in] unitNormal      Unit-length shading normal.
 * @param [in] albedo          Material's Lambertian albedo at the hit.
 * @param [in] scene           Scene used for the occlusion test.
 * @returns Linear RGB contribution from @p light to the direct lighting
 *          sum. `(0, 0, 0)` if the surface is backfacing or the light
 *          is occluded.
 */
raytracer::math::Color lambertContributionFromLight(
    const ILight& light, const raytracer::math::Vector3D& shadingPoint,
    const raytracer::math::Vector3D& unitNormal,
    const raytracer::math::Color& albedo,
    const raytracer::scene::Scene& scene) {
  const raytracer::math::Vector3D lightDir = light.getDirection(shadingPoint);
  double cosTheta = 1.0;
  if (lightDir.lengthSquared() > 0.0) {
    cosTheta = std::max(0.0, unitNormal.dot(-lightDir));
    if (cosTheta == 0.0) {
      return {0, 0, 0};
    }
  }
  const raytracer::math::Color radiance = light.illuminate(shadingPoint, scene);
  if (radiance.r == 0.0 && radiance.g == 0.0 && radiance.b == 0.0) {
    return {0, 0, 0};
  }
  return albedo * radiance * cosTheta;
}

}  // namespace

namespace raytracer::core {

components::Image MonoThreadRenderer::render(const RendererConfig& config,
                                             const Frame& frame) {
  const scene::Scene& scene = *config.scene;
  const ICamera& camera = *frame.camera;
  const math::RenderSettings& settings = config.settings;
  const int width = settings.imageWidth;
  const int height = settings.imageHeight;
  components::Image image(width, height);
  constexpr int progressLineInterval = 1;
  // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
  for (int y = 0; y < height; ++y) {
    if (progressCallback_) {
      if (y % progressLineInterval == 0) {
        progressCallback_(static_cast<double>(y) / height);
      }
    }
    // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
    for (int x = 0; x < width; ++x) {
      math::Color accumulated(0, 0, 0);
      // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
      for (int s = 0; s < settings.samplesPerPixel; ++s) {
        thread_local std::mt19937 gen(std::random_device{}());
        thread_local std::uniform_real_distribution<double> dist(0.0, 1.0);
        const double jitterU = (settings.samplesPerPixel > 1) ? dist(gen) : 0.0;
        const double jitterV = (settings.samplesPerPixel > 1) ? dist(gen) : 0.0;
        const double u = (width > 1)
                             ? (static_cast<double>(x) + jitterU) / (width - 1)
                             : 0.5;
        const double v =
            (height > 1)
                ? (static_cast<double>(height - 1 - y) + jitterV) / (height - 1)
                : 0.5;
        const math::Ray ray = camera.getRay(u, v);
        accumulated =
            accumulated + castRay(ray, scene, settings.maxDepth, true);
      }
      image.setPixel(
          x, y, accumulated / static_cast<double>(settings.samplesPerPixel));
    }
  }
  if (progressCallback_) {
    progressCallback_(1.0);
  }
  return image;
}

void MonoThreadRenderer::setProgressCallback(std::function<void(double)> fn) {
  progressCallback_ = std::move(fn);
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::castRay(const math::Ray& ray,
                                        const scene::Scene& scene, int depth,
                                        bool isPrimary) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord rec;
  if (scene.hit(ray, primaryRayTMin, std::numeric_limits<double>::infinity(),
                rec)) {
    return shade(ray, rec, scene, depth);
  }
  const bool allowEnvironment =
      isPrimary ||
      scene.getWorld().viewportMode() == scene::ViewportMode::MaterialPreview;
  if (allowEnvironment) {
    const auto background = scene.getBackground();
    if (background) {
      return background->getColor(ray);
    }
  }
  return {0, 0, 0};
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::shade(const math::Ray& inRay,
                                      const math::HitRecord& rec,
                                      const scene::Scene& scene, int depth) {
  switch (scene.getWorld().viewportMode()) {
    case scene::ViewportMode::Wireframe:
      return shadeWireframe(rec);
    case scene::ViewportMode::MaterialPreview:
      return shadeMaterialPreview(inRay, rec, scene, depth);
    case scene::ViewportMode::Rendered:
      return shadeRendered(inRay, rec, scene, depth);
  }
  return {0, 0, 0};
}

math::Color MonoThreadRenderer::shadeWireframe(const math::HitRecord& rec) {
  return normalAsColor(unitShadingNormal(rec));
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::shadeMaterialPreview(const math::Ray& inRay,
                                                     const math::HitRecord& rec,
                                                     const scene::Scene& scene,
                                                     int depth) {
  if (!rec.material) {
    return normalAsColor(unitShadingNormal(rec));
  }
  math::Color attenuation(0, 0, 0);
  math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
  if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
    const math::Color indirect = castRay(scattered, scene, depth - 1, false);
    return rec.material->emitted() + (attenuation * indirect);
  }
  return rec.material->emitted();
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MonoThreadRenderer::shadeRendered(const math::Ray& inRay,
                                              const math::HitRecord& rec,
                                              const scene::Scene& scene,
                                              int depth) {
  if (!rec.material) {
    return normalAsColor(unitShadingNormal(rec));
  }

  const math::Vector3D unitNormal = unitShadingNormal(rec);
  const math::Vector3D shadingPoint =
      rec.point + (unitNormal * math::constants::shadowRayEpsilon);

  math::Color directLighting(0, 0, 0);
  const math::Color albedo = rec.material->diffuseAlbedo();
  const bool hasDiffuseTerm =
      albedo.r > 0.0 || albedo.g > 0.0 || albedo.b > 0.0;
  if (hasDiffuseTerm) {
    for (const auto& light : scene.getLights()) {
      directLighting =
          directLighting + lambertContributionFromLight(
                               *light, shadingPoint, unitNormal, albedo, scene);
    }
  }

  math::Color attenuation(0, 0, 0);
  math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
  if (rec.material->scatter(inRay, rec, attenuation, scattered)) {
    const math::Color indirect = castRay(scattered, scene, depth - 1, false);
    return rec.material->emitted() + directLighting + (attenuation * indirect);
  }
  return rec.material->emitted() + directLighting;
}

}  // namespace raytracer::core
