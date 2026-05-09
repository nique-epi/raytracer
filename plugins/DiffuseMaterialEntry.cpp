#include "components/material/IMaterial.hpp"
#include "components/material/diffuse/DiffuseMaterial.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createMaterial(
    raytracer::core::registry::Registry<IMaterial>& registry) {
  registry.registerType("diffuse", [](const libconfig::Setting&) {
    return std::make_shared<
        raytracer::components::material::DiffuseMaterial>();
  });
}

extern "C" void destroyMaterial(gsl::owner<IMaterial*> material) {
  delete material;
}
