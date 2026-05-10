/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TranslationPlugin
*/

#include "Translation.hpp"
#include "components/Transformations/ITransformation.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<ITransformation*> createTransformations() {
  return new raytracer::components::transformation::Translation();
}

extern "C" void destroyTransformations(gsl::owner<ITransformation*> transform) {
  delete transform;
}
