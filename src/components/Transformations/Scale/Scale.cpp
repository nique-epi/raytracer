/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Scale
*/

#include "Scale.hpp"
#include <cmath>
#include "utils/math/Constants.hpp"  // NOLINT(misc-include-cleaner)
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::transformation {

Scale::Scale(const raytracer::math::Vector3D& factor) : factor_(factor) {}

void Scale::setFactor(const raytracer::math::Vector3D& factor) {
  factor_ = factor;
}

raytracer::math::Vector3D Scale::apply(
    const raytracer::math::Vector3D& point) const {
  return {point.x * factor_.x, point.y * factor_.y, point.z * factor_.z};
}

raytracer::math::Vector3D Scale::applyToNormal(
    const raytracer::math::Vector3D& normal) const {
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeX = std::abs(factor_.x) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.x;
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeY = std::abs(factor_.y) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.y;
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeZ = std::abs(factor_.z) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.z;
  const raytracer::math::Vector3D scaled{normal.x / safeX, normal.y / safeY,
                                         normal.z / safeZ};
  return scaled.normalize();
}

std::shared_ptr<ITransformation> Scale::inverse() const {
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeX = std::abs(factor_.x) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.x;
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeY = std::abs(factor_.y) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.y;
  // NOLINTNEXTLINE(cppcoreguidelines-init-variables)
  const double safeZ = std::abs(factor_.z) < raytracer::math::constants::epsilon
                           ? raytracer::math::constants::epsilon
                           : factor_.z;
  return std::make_shared<Scale>(
      raytracer::math::Vector3D{1.0 / safeX, 1.0 / safeY, 1.0 / safeZ});
}

}  // namespace raytracer::components::transformation
