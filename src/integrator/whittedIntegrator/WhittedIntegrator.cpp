/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WhittedIntegrator
*/

#include "integrator/whittedIntegrator/WhittedIntegrator.hpp"

#include <algorithm>
#include <limits>

#include "components/light/ILight.hpp"
#include "components/material/IMaterial.hpp"
#include "scene/Scene.hpp"
#include "shading_common/ShadingHelpers.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;
using raytracer::shading::common::normalAsColor;
using raytracer::shading::common::unitShadingNormal;
namespace mathConstants = raytracer::math::constants;
namespace shadingCommon = raytracer::shading::common;

Color lambertContributionFromLight(const ILight& light,
                                   const Vector3D& shadingPoint,
                                   const Vector3D& unitNormal,
                                   const Color& albedo, const Scene& scene) {
  const Vector3D lightDir = light.getDirection(shadingPoint);
  double cosTheta = 1.0;
  if (lightDir.lengthSquared() > 0.0) {
    cosTheta = std::max(0.0, unitNormal.dot(-lightDir));
    if (cosTheta == 0.0) {
      return {0, 0, 0};
    }
  }
  const Color radiance = light.illuminate(shadingPoint, scene);
  if (radiance.r == 0.0 && radiance.g == 0.0 && radiance.b == 0.0) {
    return {0, 0, 0};
  }
  return albedo * radiance * cosTheta;
}

Color castRay(const Ray& ray, const Scene& scene, int depth, bool isPrimary);

// NOLINTNEXTLINE(misc-no-recursion)
Color shade(const Ray& inRay, const HitRecord& record, const Scene& scene,
            int depth) {
  if (!record.material) {
    return normalAsColor(unitShadingNormal(record));
  }

  const Vector3D unitNormal = unitShadingNormal(record);
  const Vector3D shadingPoint =
      record.point +
      (unitNormal * mathConstants::shadowRayEpsilon);

  Color directLighting(0, 0, 0);
  const Color albedo = record.material->diffuseAlbedo();
  const bool hasDiffuseTerm =
      albedo.r > 0.0 || albedo.g > 0.0 || albedo.b > 0.0;
  if (hasDiffuseTerm) {
    for (const auto& light : scene.getLights()) {
      directLighting =
          directLighting + lambertContributionFromLight(
                               *light, shadingPoint, unitNormal, albedo, scene);
    }
  }

  Color attenuation(0, 0, 0);
  Ray scattered(Vector3D(0, 0, 0), Vector3D(0, 0, 1));
  if (record.material->scatter(inRay, record, attenuation, scattered)) {
    const Color indirect = castRay(scattered, scene, depth - 1, false);
    return record.material->emitted() + directLighting +
           (attenuation * indirect);
  }
  return record.material->emitted() + directLighting;
}

// NOLINTNEXTLINE(misc-no-recursion)
Color castRay(const Ray& ray, const Scene& scene, int depth, bool isPrimary) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  HitRecord record;
  if (scene.hit(ray, shadingCommon::primaryRayTMin,
                std::numeric_limits<double>::infinity(), record)) {
    return shade(ray, record, scene, depth);
  }
  if (isPrimary) {
    const auto background = scene.getBackground();
    if (background) {
      return background->getColor(ray);
    }
  }
  return {0, 0, 0};
}

}  // namespace

namespace raytracer::core {

math::Color WhittedIntegrator::computeRadiance(const math::Ray& ray,
                                               const scene::Scene& scene,
                                               int depth) {
  return castRay(ray, scene, depth, true);
}

}  // namespace raytracer::core
