/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone
*/

#include "Cone.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <numbers>
#include <utility>
#include "utils/math/AABB.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
// Half-angle of the cone in radians (45° default).
constexpr double defaultHalfAngle = std::numbers::pi / 4.0;
constexpr double defaultConeHeight = 1.0;
}  // namespace

namespace raytracer::components::primitives {

using raytracer::math::constants::epsilon;

Cone::Cone()
    : apex_(0.0, 0.0, 0.0),
      axis_(0.0, 1.0, 0.0),
      angle_(defaultHalfAngle),
      height_(defaultConeHeight),
      cosAngle_(std::cos(defaultHalfAngle)),
      sinAngle_(std::sin(defaultHalfAngle)),
      material_(nullptr) {}

Cone::Cone(const math::Vector3D& apex, const math::Vector3D& axis, double angle,
           double height, std::shared_ptr<IMaterial> material)
    : apex_(apex),
      axis_(axis.normalize()),
      angle_(angle),
      height_(height),
      cosAngle_(std::cos(angle)),
      sinAngle_(std::sin(angle)),
      material_(std::move(material)) {}

void Cone::setApex(const math::Vector3D& apex) { apex_ = apex; }
void Cone::setAxis(const math::Vector3D& axis) { axis_ = axis.normalize(); }

void Cone::setHalfAngle(double angle) {
  angle_ = angle;
  cosAngle_ = std::cos(angle);
  sinAngle_ = std::sin(angle);
}

void Cone::setHeight(double height) { height_ = height; }

void Cone::setMaterial(std::shared_ptr<IMaterial> material) {
  material_ = std::move(material);
}

bool Cone::computeQuadraticCoeffs(const math::Ray& ray, double& quadA,
                                   double& quadHalfB, double& quadC) const {
  const math::Vector3D originToApex = ray.getOrigin() - apex_;
  const double dirAxisProjection = ray.getDirection().dot(axis_);
  const double originAxisProjection = originToApex.dot(axis_);
  const double dirLengthSquared = ray.getDirection().dot(ray.getDirection());
  const double originDirDot = originToApex.dot(ray.getDirection());
  const double originLengthSquared = originToApex.dot(originToApex);
  const double cosAngleSquared = cosAngle_ * cosAngle_;

  quadA = (dirAxisProjection * dirAxisProjection) - (cosAngleSquared * dirLengthSquared);
  quadHalfB = (dirAxisProjection * originAxisProjection) - (cosAngleSquared * originDirDot);
  quadC = (originAxisProjection * originAxisProjection) - (cosAngleSquared * originLengthSquared);
  return std::abs(quadA) >= epsilon;
}

double Cone::findClosestValidRoot(const math::Ray& ray, double tMin,
                                   double tMax, double quadA, double quadHalfB,
                                   double quadC) const {
  const double discriminant = (quadHalfB * quadHalfB) - (quadA * quadC);
  if (discriminant < 0.0) {
    return -1.0;
  }

  const double sqrtDiscriminant = std::sqrt(discriminant);
  const double nearRoot = (-quadHalfB - sqrtDiscriminant) / quadA;
  const double farRoot = (-quadHalfB + sqrtDiscriminant) / quadA;

  auto isWithinBounds = [&](double t) -> bool {
    if (t < tMin || t > tMax) {
      return false;
    }
    const double axialProjection = (ray.at(t) - apex_).dot(axis_);
    return axialProjection >= 0.0 && axialProjection <= height_;
  };

  double closestT = tMax + 1.0;
  if (isWithinBounds(nearRoot)) {
    closestT = nearRoot;
  }
  if (isWithinBounds(farRoot) && farRoot < closestT) {
    closestT = farRoot;
  }
  return closestT > tMax ? -1.0 : closestT;
}

bool Cone::computeOutwardNormal(const math::Vector3D& hitPoint,
                                 math::Vector3D& outNormal) const {
  const math::Vector3D apexToHit = hitPoint - apex_;
  const double axialProjection = apexToHit.dot(axis_);
  const math::Vector3D lateral = apexToHit - axis_ * axialProjection;
  const double lateralLength = lateral.length();

  if (lateralLength < epsilon) {
    return false;
  }
  const math::Vector3D lateralHat = lateral / lateralLength;
  outNormal = lateralHat * cosAngle_ - axis_ * sinAngle_;
  return true;
}

bool Cone::hits(const math::Ray& ray, double tMin, double tMax,
                math::HitRecord& rec) const {
  double quadA = 0.0;
  double quadHalfB = 0.0;
  double quadC = 0.0;

  if (!computeQuadraticCoeffs(ray, quadA, quadHalfB, quadC)) {
    return false;
  }

  const double closestT = findClosestValidRoot(ray, tMin, tMax,
                                               quadA, quadHalfB, quadC);
  if (closestT < 0.0) {
    return false;
  }

  rec.t = closestT;
  rec.point = ray.at(closestT);

  math::Vector3D outNormal;
  if (!computeOutwardNormal(rec.point, outNormal)) {
    return false;
  }

  rec.setFaceNormal(ray, outNormal);
  rec.material = material_;
  return true;
}

math::AABB Cone::getBoundingBox() const {
  const math::Vector3D baseCenter = apex_ + axis_ * height_;
  const double baseRadius = height_ * std::tan(angle_);

  const math::Vector3D extent(
      baseRadius * std::sqrt(std::max(0.0, 1.0 - (axis_.x * axis_.x))),
      baseRadius * std::sqrt(std::max(0.0, 1.0 - (axis_.y * axis_.y))),
      baseRadius * std::sqrt(std::max(0.0, 1.0 - (axis_.z * axis_.z))));

  const math::AABB apexBox(apex_, apex_);
  const math::AABB baseBox(baseCenter - extent, baseCenter + extent);
  return math::AABB::surrounding(apexBox, baseBox);
}

void Cone::applyTransformation(const ITransformation& /*transform*/) {}

}  // namespace raytracer::components::primitives

