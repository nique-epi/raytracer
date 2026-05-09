#include "components/camera/ICamera.hpp"
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

extern "C" void createCamera(
    raytracer::core::registry::Registry<ICamera>& registry) {
  registry.registerType("perspective", [](const libconfig::Setting&) {
    return std::make_shared<
        raytracer::components::camera::perspective::Perspective>();
  });
}

extern "C" void destroyCamera(gsl::owner<ICamera*> camera) { delete camera; }
