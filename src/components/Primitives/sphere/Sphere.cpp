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
#include "core/registry/registry.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

namespace {
constexpr double minRadius = 1e-8;
}

Sphere::Sphere(const math::Vector3D& center, double radius,
               std::shared_ptr<IMaterial> material)
    : center_(center),
      radius_(std::max(minRadius, radius)),
      material_(std::move(material)) {}

bool Sphere::hits(const math::Ray& ray, double tMin, double tMax,
                  math::HitRecord& rec) const {
  const math::Vector3D oc = ray.getOrigin() - center_;
  const double a = ray.getDirection().dot(ray.getDirection());
  const double halfB = oc.dot(ray.getDirection());
  const double c = oc.dot(oc) - radius_ * radius_;
  const double discriminant = halfB * halfB - a * c;
  if (discriminant < 0.0) {
    return false;
  }
  const double sqrtd = std::sqrt(discriminant);
  double root = (-halfB - sqrtd) / a;
  if (root < tMin || root > tMax) {
    root = (-halfB + sqrtd) / a;
    if (root < tMin || root > tMax) {
      return false;
    }
  }
  rec.t = root;
  rec.point = ray.at(rec.t);
  const math::Vector3D outNormal = (rec.point - center_) / radius_;
  rec.setFaceNormal(ray, outNormal);
  computeUV(outNormal, rec.u, rec.v);
  rec.material = material_;
  return true;
}

math::AABB Sphere::getBoundingBox() const {
  const math::Vector3D r(radius_, radius_, radius_);
  return math::AABB(center_ - r, center_ + r);
}

void Sphere::applyTransformation(const ITransformation&) {}

void Sphere::computeUV(const math::Vector3D& p, double& u, double& v) {
  const double theta = std::acos(-p.y);
  const double phi = std::atan2(-p.z, p.x) + std::numbers::pi;
  u = phi / (2.0 * std::numbers::pi);
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
