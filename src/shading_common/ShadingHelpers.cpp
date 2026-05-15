/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Helpers shared between every integrator and every viewport shader
*/

#include "shading_common/ShadingHelpers.hpp"

#include <cmath>

#include "utils/math/Constants.hpp"

namespace raytracer::shading::common {

math::Vector3D unitShadingNormal(const math::HitRecord& record) {
  const double lengthSquared = record.normal.lengthSquared();
  if (lengthSquared <= 0.0) {
    return record.normal;
  }
  return record.normal / std::sqrt(lengthSquared);
}

math::Color normalAsColor(const math::Vector3D& normal) {
  return {(normal.x * math::constants::HALF) + math::constants::HALF,
          (normal.y * math::constants::HALF) + math::constants::HALF,
          (normal.z * math::constants::HALF) + math::constants::HALF};
}

}  // namespace raytracer::shading::common
