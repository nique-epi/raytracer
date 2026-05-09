#include "components/Primitives/IObject.hpp"
#include "components/Primitives/cone/Cone.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType("cone", [](const libconfig::Setting&) {
    return std::make_shared<raytracer::components::primitives::Cone>();
  });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
