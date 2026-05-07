/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AmbientLight
*/

#pragma once

#include "components/light/ILight.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::components::light::ambient {

class Ambient : public ILight {
 public:
  Ambient();
  Ambient(const raytracer::math::Color& color, double intensity);

  Ambient(const Ambient&) = default;
  Ambient& operator=(const Ambient&) = default;
  Ambient(Ambient&&) = default;
  Ambient& operator=(Ambient&&) = default;
  ~Ambient() override = default;

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
  raytracer::math::Color color = raytracer::math::Color(1.0, 1.0, 1.0);
  double intensity = 1.0;
};

}  // namespace raytracer::components::light::ambient
