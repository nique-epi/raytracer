/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Optics
*/

#include "Optics.hpp"
#include <cmath>
#include "Vector3D.hpp"

namespace raytracer::math::Optics {

namespace constants {
constexpr double reflectFactor = 2.0;
constexpr double one = 1.0;
constexpr double schlickExponent = 5.0;
}  // namespace constants

Vector3D reflect(const Vector3D& v, const Vector3D& n) {
  return v - n * (constants::reflectFactor * v.dot(n));
}

Vector3D refract(const Vector3D& v, const Vector3D& n, double ni_nt) {
  const Vector3D uv = v.normalize();
  const double dt = uv.dot(n);
  const double discriminant =
      constants::one - ((ni_nt * ni_nt) * (constants::one - (dt * dt)));
  if (discriminant <= 0.0) {
    return reflect(uv, n);
  }
  return uv * ni_nt - n * ((ni_nt * dt) + std::sqrt(discriminant));
}

double schlick(double cosine, double refIdx) {
  double r0 = (refIdx - constants::one) / (refIdx + constants::one);
  r0 = r0 * r0;
  return r0 + ((constants::one - r0) *
               std::pow(constants::one - cosine, constants::schlickExponent));
}

}  // namespace raytracer::math::Optics
