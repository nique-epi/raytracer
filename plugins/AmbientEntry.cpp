#include "components/light/ILight.hpp"
#include "components/light/ambient/AmbientLight.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createLights(
    raytracer::core::registry::Registry<ILight>& registry) {
  registry.registerType("ambient", [](const libconfig::Setting&) {
    return std::make_shared<
        raytracer::components::light::ambient::Ambient>();
  });
}

extern "C" void destroyLights(gsl::owner<ILight*> light) { delete light; }
