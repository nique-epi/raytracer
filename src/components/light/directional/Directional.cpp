/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Directional
*/

#include "Directional.hpp"
#include <limits>
#include "scene/Scene.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

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

bool Directional::isOccluded(const raytracer::math::Vector3D& point,
                             const raytracer::scene::Scene& scene) const {
  const raytracer::math::Ray shadowRay(point, -direction);
  raytracer::math::HitRecord rec;
  return scene.hit(shadowRay, 0.0,
                   std::numeric_limits<double>::infinity(), rec);
}

}  // namespace raytracer::components::light::directional

extern "C" gsl::owner<ILight*> createLight() {
  return new raytracer::components::light::directional::Directional();
}

extern "C" void DestroyLight(gsl::owner<ILight*> light) { delete light; }
