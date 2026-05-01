/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Directional
*/

#pragma once

#include "components/light/ILight.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::components::light::directional {

class Directional : public ILight {
 public:
  Directional();
  Directional(const raytracer::math::Vector3D& direction,
              const raytracer::math::Color& color, double intensity);

  Directional(const Directional&) = default;
  Directional& operator=(const Directional&) = default;
  Directional(Directional&&) = default;
  Directional& operator=(Directional&&) = default;
  ~Directional() override = default;

  [[nodiscard]] raytracer::math::Color illuminate(
      const raytracer::math::Vector3D& point,
      const raytracer::scene::Scene& scene) const override;

  [[nodiscard]] raytracer::math::Vector3D getDirection(
      const raytracer::math::Vector3D& point) const override;

  [[nodiscard]] double getIntensity() const override;

  [[nodiscard]] bool isOccluded(const raytracer::math::Vector3D& point,
                                const raytracer::scene::Scene& scene)
      const override;

 private:
  raytracer::math::Vector3D direction =
      raytracer::math::Vector3D(0.0, -1.0, 0.0);
  raytracer::math::Color color = raytracer::math::Color(1.0, 1.0, 1.0);
  double intensity = 1.0;
};

}  // namespace raytracer::components::light::directional
