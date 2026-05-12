/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder
*/

#include "Cylinder.hpp"
#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
#include "components/Transformations/ITransformation.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace {
constexpr double positiveInfinity = std::numeric_limits<double>::infinity();
}  // namespace

namespace raytracer::components::primitives {

using raytracer::math::constants::epsilon;

Cylinder::Cylinder(const math::Vector3D& center, const math::Vector3D& axis,
                   double radius, double height,
                   std::shared_ptr<IMaterial> material)
    : center_(center),
      axis_(axis.normalize()),
      radius_(radius),
      height_(height),
      material_(std::move(material)) {}

bool Cylinder::computeQuadraticCoeffs(const math::Ray& ray,
                                      double& perpDirLengthSq,
                                      double& perpDirOriginDot,
                                      double& perpOriginSqMinusRadiusSq) const {
  const math::Vector3D originToCenter = ray.getOrigin() - center_;
  const double rayDirAxisProjection = ray.getDirection().dot(axis_);
  const double originAxisProjection = originToCenter.dot(axis_);
  const math::Vector3D rayDirPerpComponent =
      ray.getDirection() - axis_ * rayDirAxisProjection;
  const math::Vector3D originPerp =
      originToCenter - axis_ * originAxisProjection;

  perpDirLengthSq = rayDirPerpComponent.dot(rayDirPerpComponent);
  perpDirOriginDot = rayDirPerpComponent.dot(originPerp);
  perpOriginSqMinusRadiusSq = originPerp.dot(originPerp) - (radius_ * radius_);

  return perpDirLengthSq >= epsilon;
}

double Cylinder::findClosestValidRoot(const math::Ray& ray, double tMin,
                                      double tMax, double perpDirLengthSq,
                                      double perpDirOriginDot,
                                      double perpOriginSqMinusRadiusSq) const {
  const double discriminant = (perpDirOriginDot * perpDirOriginDot) -
                              (perpDirLengthSq * perpOriginSqMinusRadiusSq);
  if (discriminant < 0.0) {
    return -1.0;
  }

  const double nearRoot =  // NOLINT(cppcoreguidelines-init-variables)
      (-perpDirOriginDot - std::sqrt(discriminant)) / perpDirLengthSq;
  const double farRoot =  // NOLINT(cppcoreguidelines-init-variables)
      (-perpDirOriginDot + std::sqrt(discriminant)) / perpDirLengthSq;

  auto isWithinBounds = [&](double rayParam) -> bool {
    if (rayParam < tMin || rayParam > tMax) {
      return false;
    }
    if (height_ <= 0.0) {
      return true;
    }
    const double axialCoord = (ray.at(rayParam) - center_).dot(axis_);
    const double halfHeight = height_ / 2.0;
    return axialCoord >= -halfHeight && axialCoord <= halfHeight;
  };

  double closestRayParam = -1.0;
  if (isWithinBounds(nearRoot)) {
    closestRayParam = nearRoot;
  }
  if (isWithinBounds(farRoot) &&
      (closestRayParam < 0.0 || farRoot < closestRayParam)) {
    closestRayParam = farRoot;
  }
  return closestRayParam;
}

bool Cylinder::hits(const math::Ray& ray, double tMin, double tMax,
                    math::HitRecord& rec) const {
  double perpDirLengthSq = 0.0;
  double perpDirOriginDot = 0.0;
  double perpOriginSqMinusRadiusSq = 0.0;

  if (!computeQuadraticCoeffs(ray, perpDirLengthSq, perpDirOriginDot,
                              perpOriginSqMinusRadiusSq)) {
    return false;
  }

  const double closestRayParam =
      findClosestValidRoot(ray, tMin, tMax, perpDirLengthSq, perpDirOriginDot,
                           perpOriginSqMinusRadiusSq);
  if (closestRayParam < 0.0) {
    return false;
  }

  rec.t = closestRayParam;
  rec.point = ray.at(closestRayParam);

  const math::Vector3D centerToHitPoint = rec.point - center_;
  const double axialProjection = centerToHitPoint.dot(axis_);
  const math::Vector3D radialComponent =
      centerToHitPoint - axis_ * axialProjection;
  const math::Vector3D outNormal = radialComponent / radius_;

  rec.setFaceNormal(ray, outNormal);
  rec.material = material_;
  return true;
}

math::AABB Cylinder::getBoundingBox() const {
  if (height_ <= 0.0) {
    return {{-positiveInfinity, -positiveInfinity, -positiveInfinity},
            {positiveInfinity, positiveInfinity, positiveInfinity}};
  }

  const double halfHeight = height_ / 2.0;
  const math::Vector3D extent(
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.x * axis_.x)))) +
          (std::abs(axis_.x) * halfHeight),
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.y * axis_.y)))) +
          (std::abs(axis_.y) * halfHeight),
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.z * axis_.z)))) +
          (std::abs(axis_.z) * halfHeight));

  return {center_ - extent, center_ + extent};
}

void Cylinder::applyTransformation(const ITransformation& /*transform*/) {}

}  // namespace raytracer::components::primitives
