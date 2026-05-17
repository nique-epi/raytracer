/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WireframeShader
*/

#include "rendering/shading/wireframe/WireframeShader.hpp"

#include <algorithm>
#include <limits>

#include "rendering/helper/RenderingHelpers.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::shading {

namespace helper = raytracer::rendering::helper;

namespace {

constexpr double wireframeMinBrightness = 0.18;
constexpr double wireframeMaxBrightness = 0.85;

}  // namespace

math::Color WireframeShader::shade(const math::Ray& ray,
                                   const scene::Scene& scene, int depth) {
  (void)depth;
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord record;
  if (!scene.hit(ray, helper::primaryRayTMin,
                 std::numeric_limits<double>::infinity(), record)) {
    return {0.0, 0.0, 0.0};
  }
  const math::Vector3D unitNormal = helper::unitShadingNormal(record);
  const math::Vector3D viewDirection = (-ray.getDirection()).normalize();
  const double facingRatio = std::max(0.0, unitNormal.dot(viewDirection));
  const double brightness =
      wireframeMinBrightness +
      ((wireframeMaxBrightness - wireframeMinBrightness) * facingRatio);
  return {brightness, brightness, brightness};
}

}  // namespace raytracer::shading
