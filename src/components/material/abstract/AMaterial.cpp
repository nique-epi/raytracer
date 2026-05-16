/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AMaterial
*/

#include "components/material/abstract/AMaterial.hpp"

#include "utils/math/Constants.hpp"
#include "utils/math/Vector3D.hpp"

raytracer::math::Color AMaterial::emitted() const {
  return {};
}

raytracer::math::Color AMaterial::diffuseAlbedo() const {
  return {};
}

raytracer::math::Color AMaterial::brdf(
    const raytracer::math::Vector3D& /*incomingDirection*/,
    const raytracer::math::Vector3D& /*outgoingDirection*/,
    const raytracer::math::Vector3D& /*normal*/) const {
  return diffuseAlbedo() / raytracer::math::constants::PI;
}
