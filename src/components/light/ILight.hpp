/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ILight
*/

#ifndef COMPONENTS_LIGHT_ILIGHT_HPP_
#define COMPONENTS_LIGHT_ILIGHT_HPP_

#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::components::light {

class ILight {
 public:
  virtual ~ILight() = default;

  [[nodiscard]] virtual raytracer::math::Color illuminate(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const = 0;

  [[nodiscard]] virtual raytracer::math::Vector3D getDirection(
      const raytracer::math::Vector3D& point) const = 0;

  [[nodiscard]] virtual double getIntensity() const = 0;

  [[nodiscard]] virtual bool isOccluded(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const = 0;
};

}  // namespace raytracer::components::light

#endif  // COMPONENTS_LIGHT_ILIGHT_HPP_
