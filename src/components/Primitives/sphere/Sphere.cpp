/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#include "Sphere.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <numbers>
#include <utility>
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

namespace {
constexpr double twoPi = 2.0 * std::numbers::pi;
}  // namespace

using raytracer::math::constants::epsilon;

Sphere::Sphere(const math::Vector3D& center, double radius,
               std::shared_ptr<IMaterial> material)
    : center_(center),
      radius_(std::max(epsilon, radius)),
      material_(std::move(material)) {}

bool Sphere::hits(const math::Ray& ray, double tMin, double tMax,
                  math::HitRecord& rec) const {
  const math::Vector3D originToCenter = ray.getOrigin() - center_;
  const double directionLengthSq = ray.getDirection().dot(ray.getDirection());
  const double halfDot = originToCenter.dot(ray.getDirection());
  const double offsetLengthSq =
      originToCenter.dot(originToCenter) - (radius_ * radius_);
  const double discriminant =
      (halfDot * halfDot) - (directionLengthSq * offsetLengthSq);
  if (discriminant < 0.0) {
    return false;
  }
  const double sqrtDiscriminant = std::sqrt(discriminant);
  double hitDistance = (-halfDot - sqrtDiscriminant) / directionLengthSq;
  if (hitDistance < tMin || hitDistance > tMax) {
    hitDistance = (-halfDot + sqrtDiscriminant) / directionLengthSq;
    if (hitDistance < tMin || hitDistance > tMax) {
      return false;
    }
  }
  rec.t = hitDistance;
  rec.point = ray.at(rec.t);
  const math::Vector3D outNormal = (rec.point - center_) / radius_;
  rec.setFaceNormal(ray, outNormal);
  computeUV(outNormal, rec.u, rec.v);
  rec.material = material_;
  return true;
}

math::AABB Sphere::getBoundingBox() const {
  const math::Vector3D r(radius_, radius_, radius_);
  return {center_ - r, center_ + r};
}

void Sphere::applyTransformation(const ITransformation& /*transform*/) {}

void Sphere::computeUV(const math::Vector3D& unitSphere, double& u, double& v) {
  const double theta = std::acos(-unitSphere.y);
  const double phi = std::atan2(-unitSphere.z, unitSphere.x) + std::numbers::pi;
  u = phi / twoPi;
  v = theta / std::numbers::pi;
}

}  // namespace raytracer::components::primitives

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "sphere", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Sphere>(
            raytracer::math::Vector3D(0.0, 0.0, 0.0), 1.0, nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
