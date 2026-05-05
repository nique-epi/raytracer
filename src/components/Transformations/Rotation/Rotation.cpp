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

Rotation::Rotation() : _axis(0.0, 0.0, 1.0) { syncRotationMatrix(); }

Rotation::Rotation(const raytracer::math::Vector3D& axis, double angle)
    : _axis(axis), _angle(angle) {
  syncRotationMatrix();
}

void Rotation::setAxis(const raytracer::math::Vector3D& axis) {
  _axis = axis;
  syncRotationMatrix();
}

void Rotation::setAngle(double angle) {
  _angle = angle;
  syncRotationMatrix();
}

void Rotation::setRotation(const raytracer::math::Vector3D& axis,
                           double angle) {
  _axis = axis;
  _angle = angle;
  syncRotationMatrix();
}

void Rotation::syncRotationMatrix() {
  _rotationMatrix = raytracer::math::Matrix4x4::rotation(_angle, _axis);
}

raytracer::math::Vector3D Rotation::apply(
    const raytracer::math::Vector3D& point) const {
  return _rotationMatrix.transformPoint(point);
}

raytracer::math::Vector3D Rotation::applyToNormal(
    const raytracer::math::Vector3D& normal) const {
  return _rotationMatrix.transformDirection(normal).normalize();
}

std::shared_ptr<ITransformation> Rotation::inverse() const {
  auto inverseTransform = std::make_shared<Rotation>(_axis, -_angle);
  return inverseTransform;
}
}  // namespace raytracer::components::transformation

extern "C" gsl::owner<ITransformation*> createTransformations() {
  return new raytracer::components::transformation::Rotation();
}

extern "C" void destroyTransformations(gsl::owner<ITransformation*> transform) {
  delete transform;
}
