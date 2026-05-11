/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidColor
*/

#include "SolidColor.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::materials::textures {

SolidColor::SolidColor(const raytracer::math::Color& color) : color_(color) {}

raytracer::math::Color SolidColor::sample(
    double /*u*/, double /*v*/,
    const raytracer::math::Vector3D& /*point*/) const {
  return color_;
}

}  // namespace raytracer::materials::textures
