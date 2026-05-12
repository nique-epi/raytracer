/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CheckerTexture
*/

#include "CheckerTexture.hpp"
#include <cmath>
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures {

CheckerTexture::CheckerTexture(const raytracer::math::Color& odd,
                               const raytracer::math::Color& even, double scale)
    : odd_(odd), even_(even), scale_(scale) {}

raytracer::math::Color CheckerTexture::sample(
    double /*u*/, double /*v*/, const raytracer::math::Vector3D& point) const {
  const double sinProduct = std::sin(scale_ * point.x) *
                            std::sin(scale_ * point.y) *
                            std::sin(scale_ * point.z);
  return sinProduct < 0.0 ? odd_ : even_;
}

}  // namespace raytracer::materials::textures
