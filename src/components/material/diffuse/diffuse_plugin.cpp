/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Diffuse material plugin entry points
*/

#include "DiffuseMaterial.hpp"
#include "utils/math/Color.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial() {
  return new DiffuseMaterial(raytracer::math::Color(1.0, 1.0, 1.0));
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
