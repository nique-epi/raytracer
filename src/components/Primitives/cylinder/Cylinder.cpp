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
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"
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
constexpr double defaultRadius = 1.0;
constexpr double defaultHeight = 1.0;
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

bool Cylinder::computeQuadraticCoeffs(const math::Ray& ray, double& a,
                                      double& halfB, double& c) const {
  const math::Vector3D w = ray.getOrigin() - center_;
  const double dAxisProj = ray.getDirection().dot(axis_);
  const double wAxisProj = w.dot(axis_);
  const math::Vector3D dPerp = ray.getDirection() - axis_ * dAxisProj;
  const math::Vector3D wPerp = w - axis_ * wAxisProj;

  a = dPerp.dot(dPerp);
  halfB = dPerp.dot(wPerp);
  c = wPerp.dot(wPerp) - (radius_ * radius_);

  return a >= epsilon;
}

double Cylinder::findClosestValidRoot(const math::Ray& ray, double tMin,
                                      double tMax, double a, double halfB,
                                      double c) const {
  const double discriminant = (halfB * halfB) - (a * c);
  if (discriminant < 0.0) {
    return -1.0;
  }

  const double sqrtDisc = std::sqrt(discriminant);
  const double nearRoot = (-halfB - sqrtDisc) / a;
  const double farRoot = (-halfB + sqrtDisc) / a;

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

  double closestT = tMax + 1.0;
  if (isWithinBounds(nearRoot)) {
    closestT = nearRoot;
  }
  if (isWithinBounds(farRoot) && farRoot < closestT) {
    closestT = farRoot;
  }
  return closestT > tMax ? -1.0 : closestT;
}

bool Cylinder::hits(const math::Ray& ray, double tMin, double tMax,
                    math::HitRecord& rec) const {
  double a = 0.0;
  double halfB = 0.0;
  double c = 0.0;

  if (!computeQuadraticCoeffs(ray, a, halfB, c)) {
    return false;
  }

  const double closestT = findClosestValidRoot(ray, tMin, tMax, a, halfB, c);
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
    constexpr double inf = std::numeric_limits<double>::infinity();
    return {{-inf, -inf, -inf}, {inf, inf, inf}};
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

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "cylinder", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Cylinder>(
            raytracer::math::Vector3D(0.0, 0.0, 0.0),
            raytracer::math::Vector3D(0.0, 1.0, 0.0), defaultRadius,
            defaultHeight, nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
