/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** GlassPlugin
*/

#include "Glass.hpp"
#include "components/material/IMaterial.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IMaterial*> createMaterial(double refractionIndex) {
  return new raytracer::components::material::Glass(refractionIndex);
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> mat) { delete mat; }
