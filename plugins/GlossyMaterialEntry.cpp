#include "components/material/IMaterial.hpp"
#include "components/material/glossy/Glossy.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createMaterial(
    raytracer::core::registry::Registry<IMaterial>& registry) {
  registry.registerType("glossy", [](const libconfig::Setting&) {
    return std::make_shared<raytracer::components::material::Glossy>();
  });
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> material) {
  delete material;
}
