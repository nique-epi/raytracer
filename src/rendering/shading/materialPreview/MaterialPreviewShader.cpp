/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialPreviewShader
*/

#include "rendering/shading/materialPreview/MaterialPreviewShader.hpp"

#include <limits>

#include "components/material/IMaterial.hpp"
#include "rendering/helper/RenderingHelpers.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
namespace helper = raytracer::rendering::helper;
}  // namespace

namespace raytracer::shading {

math::Color MaterialPreviewShader::shade(const math::Ray& ray,
                                         const scene::Scene& scene, int depth) {
  return traceRay(ray, scene, depth);
}

// NOLINTNEXTLINE(misc-no-recursion)
math::Color MaterialPreviewShader::traceRay(const math::Ray& ray,
                                            const scene::Scene& scene,
                                            int depth) {
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

  math::Color attenuation(0, 0, 0);
  math::Ray scattered(math::Vector3D(0, 0, 0), math::Vector3D(0, 0, 1));
  if (record.material->scatter(ray, record, attenuation, scattered)) {
    const math::Color indirect = traceRay(scattered, scene, depth - 1);
    return record.material->emitted() + (attenuation * indirect);
  }
  return record.material->emitted();
}

}  // namespace raytracer::shading
