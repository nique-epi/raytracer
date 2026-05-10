/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AmbientLightPlugin
*/

#include "AmbientLight.hpp"
#include "components/light/ILight.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<ILight*> createLight() {
  return new raytracer::components::light::ambient::Ambient();
}

extern "C" void DestroyLight(gsl::owner<ILight*> light) { delete light; }
