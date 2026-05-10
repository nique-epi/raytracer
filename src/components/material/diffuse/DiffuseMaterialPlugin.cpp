/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DiffuseMaterialPlugin
*/

#include "DiffuseMaterial.hpp"
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial(
    const raytracer::math::Color& albedo) {
  return new raytracer::components::material::DiffuseMaterial(albedo);
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
