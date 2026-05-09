#include "components/material/IMaterial.hpp"
#include "components/material/glass/Glass.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createMaterial(
    raytracer::core::registry::Registry<IMaterial>& registry) {
  registry.registerType("glass", [](const libconfig::Setting&) {
    return std::make_shared<raytracer::components::material::Glass>();
  });
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> material) {
  delete material;
}
