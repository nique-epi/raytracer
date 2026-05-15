/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WireframeShader
*/

#include "shading/wireframe/WireframeShader.hpp"

#include <limits>

#include "scene/Scene.hpp"
#include "shading_common/ShadingHelpers.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::shading {

math::Color WireframeShader::shade(const math::Ray& ray,
                                   const scene::Scene& scene, int depth) {
  (void)depth;
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord record;
  if (scene.hit(ray, common::primaryRayTMin,
                std::numeric_limits<double>::infinity(), record)) {
    return common::normalAsColor(common::unitShadingNormal(record));
  }
  return {0, 0, 0};
}

}  // namespace raytracer::shading
