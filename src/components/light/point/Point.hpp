/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Point
*/

#pragma once

#include "components/light/ILight.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::components::light::point {

class PointLight : public ILight {
 public:
  PointLight();
  PointLight(const raytracer::math::Vector3D& position,
             const raytracer::math::Color& color, double intensity);

  PointLight(const PointLight&) = default;
  PointLight& operator=(const PointLight&) = default;
  PointLight(PointLight&&) = default;
  PointLight& operator=(PointLight&&) = default;
  ~PointLight() override = default;

  [[nodiscard]] raytracer::math::Color illuminate(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const override;

  [[nodiscard]] raytracer::math::Vector3D getDirection(
      const raytracer::math::Vector3D& point) const override;

  [[nodiscard]] double getIntensity() const override;

  [[nodiscard]] bool isOccluded(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const override;

 private:
  raytracer::math::Vector3D position = raytracer::math::Vector3D(0.0, 1.0, 0.0);
  raytracer::math::Color color = raytracer::math::Color(1.0, 1.0, 1.0);
  double intensity = 1.0;
};

}  // namespace raytracer::components::light::point
