/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SpherePlugin
*/

#include <memory>
#include "Sphere.hpp"
#include "components/Primitives/IObject.hpp"
#include "registry/registry.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Sphere>(
            raytracer::math::Vector3D(0.0, 0.0, 0.0), 1.0, nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
