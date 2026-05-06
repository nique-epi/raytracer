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
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

namespace {
constexpr double epsilon = 1e-8;
}  // namespace

Cone::Cone(const math::Vector3D& apex, const math::Vector3D& axis, double angle,
           double height, std::shared_ptr<IMaterial> material)
    : apex_(apex),
      axis_(axis.normalize()),
      angle_(angle),
      height_(height),
      cosAngle_(std::cos(angle)),
      sinAngle_(std::sin(angle)),
      material_(std::move(material)) {}

bool Cone::hits(const math::Ray& ray, double tMin, double tMax,
                math::HitRecord& rec) const {
  const math::Vector3D w = ray.getOrigin() - apex_;

  const double dD = ray.getDirection().dot(axis_);
  const double dw = w.dot(axis_);
  const double dd = ray.getDirection().dot(ray.getDirection());
  const double wd = w.dot(ray.getDirection());
  const double ww = w.dot(w);

  const double cos2 = cosAngle_ * cosAngle_;
  const double a = dD * dD - cos2 * dd;
  const double halfB = dD * dw - cos2 * wd;
  const double c = dw * dw - cos2 * ww;

  if (std::abs(a) < epsilon) return false;

  const double discriminant = halfB * halfB - a * c;
  if (discriminant < 0.0) return false;

  const double sqrtDisc = std::sqrt(discriminant);

  auto isValidHit = [&](double t) -> bool {
    if (t < tMin || t > tMax) return false;
    const double h = (ray.at(t) - apex_).dot(axis_);
    return h >= 0.0 && h <= height_;
  };

  const double t1 = (-halfB - sqrtDisc) / a;
  const double t2 = (-halfB + sqrtDisc) / a;
  double t = tMax + 1.0;
  if (isValidHit(t1)) t = t1;
  if (isValidHit(t2) && t2 < t) t = t2;
  if (t > tMax) return false;

  rec.t = t;
  rec.point = ray.at(t);

  const math::Vector3D v = rec.point - apex_;
  const double h = v.dot(axis_);
  const math::Vector3D lateral = v - axis_ * h;
  const double lateralLen = lateral.length();

  if (lateralLen < epsilon) return false;

  const math::Vector3D lateralHat = lateral / lateralLen;
  const math::Vector3D outNormal = lateralHat * cosAngle_ - axis_ * sinAngle_;
  rec.setFaceNormal(ray, outNormal);
  rec.material = material_;
  return true;
}

math::AABB Cone::getBoundingBox() const {
  const math::Vector3D baseCenter = apex_ + axis_ * height_;
  const double baseRadius = height_ * std::tan(angle_);

  const math::Vector3D extent(
      baseRadius * std::sqrt(std::max(0.0, 1.0 - axis_.x * axis_.x)),
      baseRadius * std::sqrt(std::max(0.0, 1.0 - axis_.y * axis_.y)),
      baseRadius * std::sqrt(std::max(0.0, 1.0 - axis_.z * axis_.z)));

  const math::AABB apexBox(apex_, apex_);
  const math::AABB baseBox(baseCenter - extent, baseCenter + extent);
  return math::AABB::surrounding(apexBox, baseBox);
}

void Cone::applyTransformation(const ITransformation& /*transform*/) {}

}  // namespace raytracer::components::primitives

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "cone", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Cone>(
            raytracer::math::Vector3D(0.0, 0.0, 0.0),
            raytracer::math::Vector3D(0.0, 1.0, 0.0), std::numbers::pi / 4.0,
            1.0, nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
