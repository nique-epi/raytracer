/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SolidBackground
*/

#include "SolidBackground.hpp"

namespace raytracer::scene::background {

SolidBackground::SolidBackground(const raytracer::math::Color& color)
    : _color(color) {}

raytracer::math::Color SolidBackground::getColor(
    const raytracer::math::Ray& /*ray*/) const {
  return _color;
}

}  // namespace raytracer::scene::background
