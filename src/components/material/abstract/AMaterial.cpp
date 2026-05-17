/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AMaterial
*/

#include "components/material/abstract/AMaterial.hpp"

#include <algorithm>
#include <cmath>

#include "utils/math/Constants.hpp"
#include "utils/math/Vector3D.hpp"

AMaterial::AMaterial(raytracer::math::Color specularAlbedo, double shininess)
    : specularAlbedo_(specularAlbedo), shininess_(shininess) {}

raytracer::math::Color AMaterial::emitted() const {
  return {};
}

raytracer::math::Color AMaterial::diffuseAlbedo() const {
  return {};
}

raytracer::math::Color AMaterial::specularAlbedo() const {
  return specularAlbedo_;
}

double AMaterial::shininess() const {
  return shininess_;
}

raytracer::math::Color AMaterial::brdf(
    const raytracer::math::Vector3D& incomingDirection,
    const raytracer::math::Vector3D& outgoingDirection,
    const raytracer::math::Vector3D& normal) const {
  const raytracer::math::Color diffuse =
      diffuseAlbedo() / raytracer::math::constants::PI;
  const raytracer::math::Vector3D reflected =
      incomingDirection - (normal * (2.0 * incomingDirection.dot(normal)));
  const double alignment =
      std::max(0.0, reflected.dot(outgoingDirection));
  const raytracer::math::Color specular =
      specularAlbedo_ * std::pow(alignment, shininess_);
  return diffuse + specular;
}
