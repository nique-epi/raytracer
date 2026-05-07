/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Directional
*/

#include "Directional.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::light::directional {

Directional::Directional() = default;

Directional::Directional(const raytracer::math::Vector3D& direction,
                         const raytracer::math::Color& color, double intensity)
    : direction(direction.normalize()), color(color), intensity(intensity) {}

raytracer::math::Color Directional::illuminate(
    const raytracer::math::Vector3D& point,
    const raytracer::scene::Scene& scene) const {
  if (isOccluded(point, scene)) {
    return {0.0, 0.0, 0.0};
  }
  return color * intensity;
}

raytracer::math::Vector3D Directional::getDirection(
    const raytracer::math::Vector3D& /*point*/) const {
  return direction;
}

double Directional::getIntensity() const { return intensity; }

/**
 * @todo implement proper shadow ray occlusion test once Scene exposes a hits()
 * API.
 */
bool Directional::isOccluded(const raytracer::math::Vector3D& /*point*/,
                             const raytracer::scene::Scene& /*scene*/) const {
  //   raytracer::math::Ray shadowRay(point, -direction);
  //   return scene.hits(shadowRay, 0.0,
  //                     std::numeric_limits<double>::infinity());
  return false;
}

}  // namespace raytracer::components::light::directional

extern "C" gsl::owner<ILight*> createLight() {
  return new raytracer::components::light::directional::Directional();
}

extern "C" void DestroyLight(gsl::owner<ILight*> light) { delete light; }
