/*
 ** EPITECH PROJECT, 2026
 ** raytracer
 ** File description:
 ** TransformedObject
 */

#include "TransformedObject.hpp"
#include <algorithm>
#include <array>
#include <ranges>

namespace raytracer::components::primitives {

namespace {

math::Vector3D applyTransforms(
    const std::vector<std::shared_ptr<ITransformation>>& transforms,
    math::Vector3D point) {
  for (const auto& transform : transforms) {
    if (transform) {
      point = transform->apply(point);
    }
  }
  return point;
}

math::Vector3D applyTransformsToNormal(
    const std::vector<std::shared_ptr<ITransformation>>& transforms,
    math::Vector3D normal) {
  for (const auto& transform : transforms) {
    if (transform) {
      normal = transform->applyToNormal(normal);
    }
  }
  return normal;
}

}  // namespace

TransformedObject::TransformedObject() : primitive_(nullptr) {}

void TransformedObject::setPrimitive(
    const std::shared_ptr<IObject>& primitive) {
  primitive_ = primitive;
}

void TransformedObject::addTransform(
    const std::shared_ptr<ITransformation>& transform) {
  if (transform) {
    transforms_.push_back(transform);
  }
}

bool TransformedObject::hits(const math::Ray& ray, double tMin, double tMax,
                             math::HitRecord& rec) const {
  if (!primitive_) {
    return false;
  }
  math::Ray transformedRay = ray;
  for (const auto& transform : transforms_ | std::views::reverse) {
    if (transform) {
      auto inverse = transform->inverse();
      if (inverse) {
        transformedRay =
            math::Ray(inverse->apply(transformedRay.getOrigin()),
                      inverse->applyToNormal(transformedRay.getDirection()));
      }
    }
  }
  if (!primitive_->hits(transformedRay, tMin, tMax, rec)) {
    return false;
  }

  rec.point = applyTransforms(transforms_, rec.point);
  const math::Vector3D worldNormal =
      applyTransformsToNormal(transforms_, rec.normal);
  rec.setFaceNormal(ray, worldNormal);
  return true;
}

raytracer::math::AABB TransformedObject::getBoundingBox() const {
  if (!primitive_) {
    return {};
  }
  const math::AABB localBox = primitive_->getBoundingBox();
  const std::array<math::Vector3D, 8> corners = {{
      {localBox.min.x, localBox.min.y, localBox.min.z},
      {localBox.min.x, localBox.min.y, localBox.max.z},
      {localBox.min.x, localBox.max.y, localBox.min.z},
      {localBox.min.x, localBox.max.y, localBox.max.z},
      {localBox.max.x, localBox.min.y, localBox.min.z},
      {localBox.max.x, localBox.min.y, localBox.max.z},
      {localBox.max.x, localBox.max.y, localBox.min.z},
      {localBox.max.x, localBox.max.y, localBox.max.z},
  }};
  math::Vector3D transformedCorner = applyTransforms(transforms_, corners[0]);
  math::Vector3D min = transformedCorner;
  math::Vector3D max = transformedCorner;
  for (const auto& corner : corners) {
    const math::Vector3D p = applyTransforms(transforms_, corner);
    min.x = std::min(min.x, p.x);
    min.y = std::min(min.y, p.y);
    min.z = std::min(min.z, p.z);
    max.x = std::max(max.x, p.x);
    max.y = std::max(max.y, p.y);
    max.z = std::max(max.z, p.z);
  }
  return {min, max};
}

void TransformedObject::applyTransformation(const ITransformation& transform) {
  if (primitive_) {
    primitive_->applyTransformation(transform);
  }
}

}  // namespace raytracer::components::primitives
