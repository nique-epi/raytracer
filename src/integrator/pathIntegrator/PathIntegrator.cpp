/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PathIntegrator
*/

#include "integrator/pathIntegrator/PathIntegrator.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
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

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;
using raytracer::scene::ViewportMode;

constexpr double primaryRayTMin = 0.001;
constexpr double normalDebugHalf = 0.5;

Vector3D unitShadingNormal(const HitRecord& record) {
  const double lengthSquared = record.normal.lengthSquared();
  if (lengthSquared <= 0.0) {
    return record.normal;
  }
  return record.normal / std::sqrt(lengthSquared);
}

Color normalAsColor(const Vector3D& normal) {
  return {(normal.x * normalDebugHalf) + normalDebugHalf,
          (normal.y * normalDebugHalf) + normalDebugHalf,
          (normal.z * normalDebugHalf) + normalDebugHalf};
}

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

Color shadeWireframe(const HitRecord& record) {
  return normalAsColor(unitShadingNormal(record));
}

// NOLINTNEXTLINE(misc-no-recursion)
Color shadeMaterialPreview(const Ray& inRay, const HitRecord& record,
                           const Scene& scene, int depth) {
  if (!record.material) {
    return normalAsColor(unitShadingNormal(record));
  }
  Color attenuation(0, 0, 0);
  Ray scattered(Vector3D(0, 0, 0), Vector3D(0, 0, 1));
  if (record.material->scatter(inRay, record, attenuation, scattered)) {
    const Color indirect =
        castRay(scattered, scene, depth - 1, false);
    return record.material->emitted() + (attenuation * indirect);
  }
  return record.material->emitted();
}

// NOLINTNEXTLINE(misc-no-recursion)
Color shadeRendered(const Ray& inRay, const HitRecord& record,
                    const Scene& scene, int depth) {
  if (!record.material) {
    return normalAsColor(unitShadingNormal(record));
  }

  const Vector3D unitNormal = unitShadingNormal(record);
  const Vector3D shadingPoint =
      record.point +
      (unitNormal * raytracer::math::constants::shadowRayEpsilon);

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
    const Color indirect =
        castRay(scattered, scene, depth - 1, false);
    return record.material->emitted() + directLighting +
           (attenuation * indirect);
  }
  return record.material->emitted() + directLighting;
}

// NOLINTNEXTLINE(misc-no-recursion)
Color shade(const Ray& inRay, const HitRecord& record, const Scene& scene,
            int depth) {
  switch (scene.getWorld().viewportMode()) {
    case ViewportMode::Wireframe:
      return shadeWireframe(record);
    case ViewportMode::MaterialPreview:
      return shadeMaterialPreview(inRay, record, scene, depth);
    case ViewportMode::Rendered:
      return shadeRendered(inRay, record, scene, depth);
  }
  return {0, 0, 0};
}

// NOLINTNEXTLINE(misc-no-recursion)
Color castRay(const Ray& ray, const Scene& scene, int depth, bool isPrimary) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  HitRecord record;
  if (scene.hit(ray, primaryRayTMin, std::numeric_limits<double>::infinity(),
                record)) {
    return shade(ray, record, scene, depth);
  }
  const bool allowEnvironment = isPrimary || scene.getWorld().viewportMode() ==
                                                 ViewportMode::MaterialPreview;
  if (allowEnvironment) {
    const auto background = scene.getBackground();
    if (background) {
      return background->getColor(ray);
    }
  }
  return {0, 0, 0};
}

}  // namespace

namespace raytracer::core {

math::Color PathIntegrator::computeRadiance(const math::Ray& ray,
                                            const scene::Scene& scene,
                                            int depth) {
  return castRay(ray, scene, depth, true);
}

}  // namespace raytracer::core
