/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Rotation
*/

#pragma once

#include "components/Transformations/ITransformation.hpp"
#include "utils/math/Matrix.hpp"

namespace raytracer::components::transformation {

class Rotation : public ITransformation {
 public:
  Rotation();
  Rotation(const raytracer::math::Vector3D& axis, double angle);
  ~Rotation() override = default;

    Rotation(const Rotation& other) = default;
    Rotation& operator=(const Rotation& other) = default;
    Rotation(Rotation&& other) noexcept = default;
    Rotation& operator=(Rotation&& other) noexcept = default;

  void setAxis(const raytracer::math::Vector3D& axis);
  void setAngle(double angle);
  void setRotation(const raytracer::math::Vector3D& axis, double angle);

  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  void syncRotationMatrix();

  raytracer::math::Vector3D _axis;
  double _angle{0.0};
  raytracer::math::Matrix4x4 _rotationMatrix;
};
}  // namespace raytracer::components::transformation
