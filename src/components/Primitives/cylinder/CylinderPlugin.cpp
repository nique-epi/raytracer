/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CylinderPlugin
*/

#include "Cylinder.hpp"
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" gsl::owner<IObject*> createPrimitive(
    const raytracer::math::Vector3D& center,
    const raytracer::math::Vector3D& axis, double radius, double height) {
  return new raytracer::components::primitives::Cylinder(center, axis, radius,
                                                         height, nullptr);
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
