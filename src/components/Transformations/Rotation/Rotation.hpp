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
  Rotation()
      : _axis(0.0, 0.0, 1.0),
        _rotationMatrix(raytracer::math::Matrix4x4::identity()) {}
  ~Rotation() override = default;

    Rotation(const Rotation& other) = default;
    Rotation& operator=(const Rotation& other) = default;
    Rotation(Rotation&& other) noexcept = default;
    Rotation& operator=(Rotation&& other) noexcept = default;

  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  raytracer::math::Vector3D _axis;
  double _angle{0.0};
  raytracer::math::Matrix4x4 _rotationMatrix;
};
}  // namespace raytracer::components::transformation
