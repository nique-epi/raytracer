/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WhittedIntegrator
*/

#include "rendering/integrator/whittedIntegrator/WhittedIntegrator.hpp"
#include <algorithm>
#include <limits>
#include <mutex>
#include "components/light/ILight.hpp"
#include "components/material/IMaterial.hpp"
#include "rendering/helper/HemisphereSampler.hpp"
#include "rendering/helper/RenderingHelpers.hpp"
#include "scene/Scene.hpp"
#include "scene/World.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

using raytracer::math::Color;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;
namespace constants = raytracer::math::constants;

Color directLightingContribution(const ILight& light, const IMaterial& material,
                                 const Vector3D& shadingPoint,
                                 const Vector3D& unitNormal,
                                 const Vector3D& viewDirection,
                                 const Scene& scene) {
  const Vector3D lightDirection = light.getDirection(shadingPoint);
  double cosTheta = 1.0;
  Color brdfValue = material.diffuseAlbedo();

  if (lightDirection.lengthSquared() > 0.0) {
    cosTheta = std::max(0.0, unitNormal.dot(-lightDirection));
    if (cosTheta == 0.0) {
      return {0, 0, 0};
    }
    brdfValue = material.brdf(lightDirection, viewDirection, unitNormal);
  }
  const Color radiance = light.illuminate(shadingPoint, scene);
  if (radiance.r == 0.0 && radiance.g == 0.0 && radiance.b == 0.0) {
    return {0, 0, 0};
  }
  return brdfValue * radiance * cosTheta;
}

}  // namespace

namespace raytracer::core {

namespace helper = raytracer::rendering::helper;

math::Color WhittedIntegrator::computeRadiance(const math::Ray& ray,
                                               const scene::Scene& scene,
                                               int depth) {
  return castRay(ray, scene, depth, true);
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color WhittedIntegrator::castRay(const math::Ray& ray,
                                       const scene::Scene& scene, int depth,
                                       bool isPrimary) {
  if (depth <= 0) {
    return {0, 0, 0};
  }

  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord record;

  if (!scene.hit(ray, helper::primaryRayTMin,
                 std::numeric_limits<double>::infinity(), record)) {
    const auto background = scene.getBackground();
    if (background) {
      return background->getColor(ray);
    }
    return {0, 0, 0};
  }

  if (!record.material) {
    return helper::normalAsColor(helper::unitShadingNormal(record));
  }

  const math::Vector3D unitNormal = helper::unitShadingNormal(record);
  const math::Vector3D shadingPoint =
      record.point + (unitNormal * constants::shadowRayEpsilon);

  math::Color directLighting(0, 0, 0);
  const math::Color albedo = record.material->diffuseAlbedo();
  const math::Color specularAlbedo = record.material->specularAlbedo();
  const bool hasDirectLightingTerm =
      albedo.r > 0.0 || albedo.g > 0.0 || albedo.b > 0.0 ||
      specularAlbedo.r > 0.0 || specularAlbedo.g > 0.0 ||
      specularAlbedo.b > 0.0;
  if (hasDirectLightingTerm) {
    const math::Vector3D viewDirection = (-ray.getDirection()).normalize();
    for (const auto& light : scene.getLights()) {
      directLighting =
          directLighting + directLightingContribution(*light, *record.material,
                                                      shadingPoint, unitNormal,
                                                      viewDirection, scene);
    }
  }

  math::Color indirectLighting(0, 0, 0);
  math::Color attenuation(0, 0, 0);
  math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));

  if (record.material->scatter(ray, record, attenuation, scattered)) {
    indirectLighting =
        attenuation * castRay(scattered, scene, depth - 1, false);
  }

  math::Color radiance =
      record.material->emitted() + directLighting + indirectLighting;

  if (isPrimary) {
    const auto& aoSettings = scene.getWorld().ambientOcclusion();
    const double occlusion =
        computeAmbientOcclusion(shadingPoint, unitNormal, scene, aoSettings);
    radiance = radiance * occlusion;
  }
  return radiance;
}

double WhittedIntegrator::computeAmbientOcclusion(
    const math::Vector3D& shadingPoint, const math::Vector3D& unitNormal,
    const scene::Scene& scene,
    const scene::AmbientOcclusionSettings& settings) {
  if (!settings.enabled || settings.samples <= 0 ||
      settings.intensity <= 0.0) {
    return 1.0;
  }

  static std::once_flag settingsLoggedOnceFlag;
  std::call_once(settingsLoggedOnceFlag, [&] {
    logger_.info("ambient occlusion engaged: samples=", settings.samples,
                 ", radius=", settings.radius,
                 ", intensity=", settings.intensity);
  });

  int blockedCount = 0;
  for (int sampleIndex = 0; sampleIndex < settings.samples; ++sampleIndex) {
    const math::Vector3D sampleDirection =
        helper::sampleCosineHemisphere(unitNormal);
    const math::Ray occlusionRay(shadingPoint, sampleDirection);
    math::HitRecord occlusionRecord;
    if (scene.hit(occlusionRay, helper::primaryRayTMin, settings.radius,
                  occlusionRecord)) {
      ++blockedCount;
    }
  }

  const double visibleFraction = 1.0 - (static_cast<double>(blockedCount) /
                                        static_cast<double>(settings.samples));
  const double factor = 1.0 - (settings.intensity * (1.0 - visibleFraction));

  logger_.trace("ambient occlusion factor=", factor, " blocked=", blockedCount,
                '/', settings.samples, " at point=(", shadingPoint.x, ',',
                shadingPoint.y, ',', shadingPoint.z, ')');
  return factor;
}

}  // namespace raytracer::core
