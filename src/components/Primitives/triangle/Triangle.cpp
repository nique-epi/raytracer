/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#include "components/Primitives/triangle/Triangle.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include "components/Primitives/IObject.hpp"
#include "core/registry/registry.hpp"
#include "utils/math/Constants.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

using raytracer::math::constants::epsilon;

Triangle::Triangle() = default;

Triangle::Triangle(const math::Point3D& vertex0, const math::Point3D& vertex1,
                   const math::Point3D& vertex2,
                   std::shared_ptr<IMaterial> material)
    : material_(std::move(material)),
      vertex0_(vertex0),
      vertex1_(vertex1),
      vertex2_(vertex2) {}

Triangle::~Triangle() = default;

bool Triangle::hits(const raytracer::math::Ray& ray, double tMin, double tMax,
                    raytracer::math::HitRecord& rec) const {
  const math::Vector3D edge1 = vertex1_ - vertex0_;
  const math::Vector3D edge2 = vertex2_ - vertex0_;
  const math::Vector3D vectorProduct = ray.getDirection().cross(edge2);
  const double determinant = edge1.dot(vectorProduct);
  if (std::abs(determinant) < epsilon) {
    return false;
  }

  const double invDeterminant = 1.0 / determinant;
  const math::Vector3D tvec = ray.getOrigin() - vertex0_;
  const double u = invDeterminant * tvec.dot(vectorProduct);
  if (u < 0.0 || u > 1.0) {
    return false;
  }

  const math::Vector3D qvec = tvec.cross(edge1);
  const double v = invDeterminant * ray.getDirection().dot(qvec);
  if (v < 0.0 || (u + v) > 1.0) {
    return false;
  }

  const double hitDistance = invDeterminant * edge2.dot(qvec);
  if (hitDistance < tMin || hitDistance > tMax) {
    return false;
  }

  const math::Vector3D outNormal = edge1.cross(edge2);
  if (outNormal.lengthSquared() < epsilon) {
    return false;
  }

  rec.t = hitDistance;
  rec.point = ray.at(rec.t);
  rec.setFaceNormal(ray, outNormal.normalize());
  rec.u = u;
  rec.v = v;
  rec.material = material_;
  return true;
}

raytracer::math::AABB Triangle::getBoundingBox() const {
  const math::Vector3D min(std::min({vertex0_.x, vertex1_.x, vertex2_.x}),
                           std::min({vertex0_.y, vertex1_.y, vertex2_.y}),
                           std::min({vertex0_.z, vertex1_.z, vertex2_.z}));
  const math::Vector3D max(std::max({vertex0_.x, vertex1_.x, vertex2_.x}),
                           std::max({vertex0_.y, vertex1_.y, vertex2_.y}),
                           std::max({vertex0_.z, vertex1_.z, vertex2_.z}));
  return {min, max};
}

void Triangle::applyTransformation(const ITransformation& transformation) {
  vertex0_ = transformation.apply(vertex0_);
  vertex1_ = transformation.apply(vertex1_);
  vertex2_ = transformation.apply(vertex2_);
}

}  // namespace raytracer::components::primitives

extern "C" void createPrimitive(
    raytracer::core::registry::Registry<IObject>& registry) {
  registry.registerType(
      "triangle", [](const libconfig::Setting&) -> std::shared_ptr<IObject> {
        return std::make_shared<raytracer::components::primitives::Triangle>(
            raytracer::math::Point3D(0.0, 0.0, 0.0),
            raytracer::math::Point3D(1.0, 0.0, 0.0),
            raytracer::math::Point3D(0.0, 1.0, 0.0), nullptr);
      });
}

extern "C" void destroyPrimitive(gsl::owner<IObject*> obj) { delete obj; }
