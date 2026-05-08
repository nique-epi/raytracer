/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
*/

#include "Plane.hpp"
#include <cmath>
#include <limits>
#include <memory>
#include <utility>
#include "components/Primitives/IObject.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/Constants.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

using raytracer::math::constants::epsilon;

Plane::Plane(const math::Vector3D& point, const math::Vector3D& normal,
             std::shared_ptr<IMaterial> material)
    : point_(point),
      normal_(normal.normalize()),
      material_(std::move(material)) {}

bool Plane::hits(const math::Ray& ray, double tMin, double tMax,
                 math::HitRecord& rec) const {
  const double denom = ray.getDirection().dot(normal_);
  if (std::abs(denom) < epsilon) {
    return false;
  }

  const double rayParameter = (point_ - ray.getOrigin()).dot(normal_) / denom;
  if (rayParameter < tMin || rayParameter > tMax) {
    return false;
  }

  rec.t = rayParameter;
  rec.point = ray.at(rayParameter);
  rec.setFaceNormal(ray, normal_);
  rec.material = material_;
  return true;
}

math::AABB Plane::getBoundingBox() const {
  constexpr double inf = std::numeric_limits<double>::infinity();
  return {math::Vector3D(-inf, -inf, -inf), math::Vector3D(inf, inf, inf)};
}

void Plane::applyTransformation(const ITransformation& transform) {
  point_ = transform.apply(point_);
  normal_ = transform.applyToNormal(normal_).normalize();
}

}  // namespace raytracer::components::primitives

extern "C" gsl::owner<IObject*> createPrimitive(
    const raytracer::math::Vector3D& point,
    const raytracer::math::Vector3D& normal) {
  return new raytracer::components::primitives::Plane(point, normal, nullptr);
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
