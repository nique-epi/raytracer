/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PlanePlugin
*/

#include "Plane.hpp"
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IObject*> createPrimitive(
    const raytracer::math::Vector3D& point,
    const raytracer::math::Vector3D& normal) {
  return new raytracer::components::primitives::Plane(point, normal, nullptr);
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
