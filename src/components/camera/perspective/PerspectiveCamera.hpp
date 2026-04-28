/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** perspectiveCamera
*/

#pragma once

#include "components/camera/ICamera.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::camera::perspective {

constexpr double DEFAULT_FOV = 90.0;

class Perspective : public ICamera {
 public:
  Perspective();
  Perspective(const raytracer::math::Vector3D& lookFrom,
              const raytracer::math::Vector3D& lookAt,
              const raytracer::math::Vector3D& vup, double vfov,
              double aspectRatio, double aperture = 0.0,
              double focusDist = 1.0);

  Perspective(const Perspective&) = default;
  Perspective& operator=(const Perspective&) = default;
  Perspective(Perspective&&) = default;
  Perspective& operator=(Perspective&&) = default;
  ~Perspective() override = default;

  [[nodiscard]] raytracer::math::Ray getRay(double u, double v) const override;
  void setResolution(int width, int height) override;

 private:
  void updateCamera();
  [[nodiscard]] raytracer::math::Vector3D randomInUnitDisk() const;

  raytracer::math::Vector3D position = raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D target = raytracer::math::Vector3D(0.0, 0.0, -1.0);
  raytracer::math::Vector3D up = raytracer::math::Vector3D(0.0, 1.0, 0.0);
  double fov = DEFAULT_FOV;
  double aspectRatio = 1.0;
  double aperture = 0.0;
  double focusDist = 1.0;

  raytracer::math::Vector3D u = raytracer::math::Vector3D(1.0, 0.0, 0.0);
  raytracer::math::Vector3D v = raytracer::math::Vector3D(0.0, 1.0, 0.0);
  raytracer::math::Vector3D w = raytracer::math::Vector3D(0.0, 0.0, 1.0);
  raytracer::math::Vector3D lowerLeftCorner =
      raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D horizontal =
      raytracer::math::Vector3D(0.0, 0.0, 0.0);
  raytracer::math::Vector3D vertical = raytracer::math::Vector3D(0.0, 0.0, 0.0);
};

}  // namespace raytracer::components::camera::perspective
