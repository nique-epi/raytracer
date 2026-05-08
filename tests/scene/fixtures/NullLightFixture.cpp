/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** NullLightFixture
*/

#include "NullLightFixture.hpp"

raytracer::math::Color NullLightFixture::illuminate(
    const raytracer::math::Vector3D&, const raytracer::scene::Scene&) const {
  return {};
}

raytracer::math::Vector3D NullLightFixture::getDirection(
    const raytracer::math::Vector3D&) const {
  return {};
}

double NullLightFixture::getIntensity() const { return 0.0; }

bool NullLightFixture::isOccluded(const raytracer::math::Vector3D&,
                                  const raytracer::scene::Scene&) const {
  return false;
}
