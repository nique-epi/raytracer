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

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

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
  const double dAxisProj = ray.getDirection().dot(axis_);
  const double originAxisProj = originToCenter.dot(axis_);
  const math::Vector3D dPerp = ray.getDirection() - axis_ * dAxisProj;
  const math::Vector3D originPerp = originToCenter - axis_ * originAxisProj;

  perpDirLengthSq = dPerp.dot(dPerp);
  perpDirOriginDot = dPerp.dot(originPerp);
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

  auto isWithinBounds = [&](double t) -> bool {
    if (t < tMin || t > tMax) {
      return false;
    }
    if (height_ <= 0.0) {
      return true;
    }
    const double axialCoord = (ray.at(t) - center_).dot(axis_);
    const double halfH = height_ / 2.0;
    return axialCoord >= -halfH && axialCoord <= halfH;
  };

  double closestT = -1.0;
  if (isWithinBounds(nearRoot)) {
    closestT = nearRoot;
  }
  if (isWithinBounds(farRoot) && (closestT < 0.0 || farRoot < closestT)) {
    closestT = farRoot;
  }
  return closestT;
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

  const double closestT =
      findClosestValidRoot(ray, tMin, tMax, perpDirLengthSq, perpDirOriginDot,
                           perpOriginSqMinusRadiusSq);
  if (closestT < 0.0) {
    return false;
  }

  rec.t = closestT;
  rec.point = ray.at(closestT);

  const math::Vector3D toPoint = rec.point - center_;
  const double axialProj = toPoint.dot(axis_);
  const math::Vector3D radial = toPoint - axis_ * axialProj;
  const math::Vector3D outNormal = radial / radius_;

  rec.setFaceNormal(ray, outNormal);
  rec.material = material_;
  return true;
}

math::AABB Cylinder::getBoundingBox() const {
  if (height_ <= 0.0) {
    return {{-positiveInfinity, -positiveInfinity, -positiveInfinity},
            {positiveInfinity, positiveInfinity, positiveInfinity}};
  }

  const double halfH = height_ / 2.0;
  const math::Vector3D extent(
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.x * axis_.x)))) +
          (std::abs(axis_.x) * halfH),
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.y * axis_.y)))) +
          (std::abs(axis_.y) * halfH),
      (radius_ * std::sqrt(std::max(0.0, 1.0 - (axis_.z * axis_.z)))) +
          (std::abs(axis_.z) * halfH));

  return {center_ - extent, center_ + extent};
}

void Cylinder::applyTransformation(const ITransformation& /*transform*/) {}

}  // namespace raytracer::components::primitives

extern "C" gsl::owner<IObject*> createPrimitive(
    const raytracer::math::Vector3D& center,
    const raytracer::math::Vector3D& axis, double radius, double height) {
  return new raytracer::components::primitives::Cylinder(center, axis, radius,
                                                         height, nullptr);
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
