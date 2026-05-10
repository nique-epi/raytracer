/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GlossyPlugin
*/

#include "Glossy.hpp"
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial(
    double fuzz, const raytracer::math::Color& albedo) {
  return new raytracer::components::material::Glossy(fuzz, albedo);
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
