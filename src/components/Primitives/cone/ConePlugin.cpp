/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConePlugin
*/

#include <memory>
#include <numbers>
#include "Cone.hpp"
#include "components/Primitives/IObject.hpp"
#include "registry/registry.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
constexpr double defaultAngle = std::numbers::pi / 4.0;
constexpr double defaultHeight = 1.0;
}  // namespace

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "cone", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Cone>(
            raytracer::math::Vector3D(0.0, 0.0, 0.0),
            raytracer::math::Vector3D(0.0, 1.0, 0.0), defaultAngle,
            defaultHeight, nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
