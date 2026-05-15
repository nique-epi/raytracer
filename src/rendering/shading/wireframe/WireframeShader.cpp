/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** WireframeShader
*/

#include "rendering/shading/wireframe/WireframeShader.hpp"

#include <limits>

#include "rendering/helper/RenderingHelpers.hpp"
#include "scene/Scene.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::shading {

namespace helper = raytracer::rendering::helper;

math::Color WireframeShader::shade(const math::Ray& ray,
                                   const scene::Scene& scene, int depth) {
  (void)depth;
  // NOLINTNEXTLINE(misc-const-correctness)
  math::HitRecord record;
  if (scene.hit(ray, helper::primaryRayTMin,
                std::numeric_limits<double>::infinity(), record)) {
    return helper::normalAsColor(helper::unitShadingNormal(record));
  }
  return {0, 0, 0};
}

}  // namespace raytracer::shading
