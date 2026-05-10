/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** DirectionalPlugin
*/

#include "Directional.hpp"
#include "components/light/ILight.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<ILight*> createLight() {
  return new raytracer::components::light::directional::Directional();
}

extern "C" void DestroyLight(gsl::owner<ILight*> light) { delete light; }
