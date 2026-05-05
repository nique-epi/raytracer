/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Rotation
*/

#include "Rotation.hpp"
#include "utils/math/Matrix.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::transformation {

raytracer::math::Vector3D Rotation::apply(
    const raytracer::math::Vector3D& point) const {
  return _rotationMatrix.transformPoint(point);
}

raytracer::math::Vector3D Rotation::applyToNormal(
    const raytracer::math::Vector3D& normal) const {
  return _rotationMatrix.transformDirection(normal).normalize();
}

std::shared_ptr<ITransformation> Rotation::inverse() const {
  auto inverseTransform = std::make_shared<Rotation>();
  inverseTransform->_axis = _axis;
  inverseTransform->_angle = -_angle;
  inverseTransform->_rotationMatrix =
      raytracer::math::Matrix4x4::rotation(-_angle, _axis);
  return inverseTransform;
}
}  // namespace raytracer::components::transformation

extern "C" gsl::owner<ITransformation*> createTransformations() {
  return new raytracer::components::transformation::Rotation();
}

extern "C" void DestroyTransformations(gsl::owner<ITransformation*> transform) {
  delete transform;
}
