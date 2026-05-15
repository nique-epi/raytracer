/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialPreviewShader
*/

#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"

#include <limits>

#include "components/material/IMaterial.hpp"
#include "scene/Scene.hpp"
#include "rendering/helper/RenderingHelpers.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {

using raytracer::math::Color;
using raytracer::math::HitRecord;
using raytracer::math::Ray;
using raytracer::math::Vector3D;
using raytracer::scene::Scene;
using raytracer::rendering::helper::normalAsColor;
using raytracer::rendering::helper::unitShadingNormal;
namespace shadingCommon = raytracer::rendering::helper;

// NOLINTNEXTLINE(misc-no-recursion)
Color shade(const Ray& ray, const Scene& scene, int depth);

// NOLINTNEXTLINE(misc-no-recursion)
Color shadeHit(const Ray& inRay, const HitRecord& record, const Scene& scene,
               int depth) {
  if (!record.material) {
    return normalAsColor(unitShadingNormal(record));
  }
  Color attenuation(0, 0, 0);
  Ray scattered(Vector3D(0, 0, 0), Vector3D(0, 0, 1));
  if (record.material->scatter(inRay, record, attenuation, scattered)) {
    const Color indirect = shade(scattered, scene, depth - 1);
    return record.material->emitted() + (attenuation * indirect);
  }
  return record.material->emitted();
}

// NOLINTNEXTLINE(misc-no-recursion)
Color shade(const Ray& ray, const Scene& scene, int depth) {
  if (depth <= 0) {
    return {0, 0, 0};
  }
  // NOLINTNEXTLINE(misc-const-correctness)
  HitRecord record;
  if (scene.hit(ray, shadingCommon::primaryRayTMin,
                std::numeric_limits<double>::infinity(), record)) {
    return shadeHit(ray, record, scene, depth);
  }
  const auto background = scene.getBackground();
  if (background) {
    return background->getColor(ray);
  }
  return {0, 0, 0};
}

}  // namespace

namespace raytracer::shading {

math::Color MaterialPreviewShader::shade(const math::Ray& ray,
                                         const scene::Scene& scene, int depth) {
  return ::shade(ray, scene, depth);
}

}  // namespace raytracer::shading
