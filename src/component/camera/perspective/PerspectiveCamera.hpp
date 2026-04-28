/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** perspectiveCamera
*/

#pragma once

#include "component/camera/ICamera.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::component::camera {

constexpr double DEFAULT_FOV = 90.0;

class Perspective : public ICamera {
 public:
  Perspective();
  Perspective(const Math::Vector3D& lookFrom, const Math::Vector3D& lookAt,
              const Math::Vector3D& vup, double vfov, double aspectRatio,
              double aperture = 0.0, double focusDist = 1.0);

  Perspective(const Perspective&) = default;
  Perspective& operator=(const Perspective&) = default;
  Perspective(Perspective&&) = default;
  Perspective& operator=(Perspective&&) = default;
  ~Perspective() override = default;

  [[nodiscard]] Math::Ray getRay(double u, double v) const override;
  void setResolution(int width, int height) override;

 private:
  void updateCamera();
  [[nodiscard]] Math::Vector3D randomInUnitDisk() const;

  Math::Vector3D position = Math::Vector3D(0.0, 0.0, 0.0);
  Math::Vector3D target = Math::Vector3D(0.0, 0.0, -1.0);
  Math::Vector3D up = Math::Vector3D(0.0, 1.0, 0.0);
  double fov = DEFAULT_FOV;
  double aspectRatio = 1.0;
  double aperture = 0.0;
  double focusDist = 1.0;

  Math::Vector3D u = Math::Vector3D(1.0, 0.0, 0.0);
  Math::Vector3D v = Math::Vector3D(0.0, 1.0, 0.0);
  Math::Vector3D w = Math::Vector3D(0.0, 0.0, 1.0);
  Math::Vector3D lowerLeftCorner = Math::Vector3D(0.0, 0.0, 0.0);
  Math::Vector3D horizontal = Math::Vector3D(0.0, 0.0, 0.0);
  Math::Vector3D vertical = Math::Vector3D(0.0, 0.0, 0.0);
};

}  // namespace raytracer::component::camera
