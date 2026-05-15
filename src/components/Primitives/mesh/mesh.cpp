/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** mesh
*/

#include "components/Primitives/mesh/mesh.hpp"
#include <memory>
#include <utility>
#include "components/Primitives/BVH/BVHNode.hpp"
#include "components/Primitives/IObject.hpp"

namespace gsl {
template <typename T>
using owner = T;
}  // namespace gsl

namespace raytracer::components::primitives {

Mesh::Mesh(std::vector<std::shared_ptr<Triangle>> triangles)
    : triangles_(std::move(triangles)) {}

void Mesh::addTriangle(std::shared_ptr<Triangle> triangle) {
  triangles_.push_back(std::move(triangle));
  bboxDirty_ = true;
}

bool Mesh::hits(const raytracer::math::Ray& ray, double tMin, double tMax,
                raytracer::math::HitRecord& rec) const {
  if (!getBoundingBox().hit(ray, tMin, tMax)) {
    return false;
  }

  if (internalBVH_) {
    return internalBVH_->hits(ray, tMin, tMax, rec);
  }

  bool hitAnything = false;
  double closestSoFar = tMax;

  for (const auto& triangle : triangles_) {
    raytracer::math::HitRecord tempRec;
    if (triangle->hits(ray, tMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      rec = tempRec;
    }
  }
  return hitAnything;
}

raytracer::math::AABB Mesh::getBoundingBox() const {
  if (!bboxDirty_) {
    return cachedBox_;
  }
  if (triangles_.empty()) {
    return {raytracer::math::Vector3D(0.0, 0.0, 0.0),
            raytracer::math::Vector3D(0.0, 0.0, 0.0)};
  }
  raytracer::math::AABB box = triangles_[0]->getBoundingBox();
  for (size_t i = 1; i < triangles_.size(); ++i) {
    box = raytracer::math::AABB::surrounding(box,
                                             triangles_[i]->getBoundingBox());
  }
  bboxDirty_ = false;
  cachedBox_ = box;
  return cachedBox_;
}

void Mesh::applyTransformation(const ITransformation& transform) {
  for (auto& triangle : triangles_) {
    triangle->applyTransformation(transform);
  }
  bboxDirty_ = true;

  internalBVH_.reset();
}

void Mesh::buildAccelerationStructure() {
  if (triangles_.empty()) {
    internalBVH_.reset();
    return;
  }

  if (internalBVH_) {
    return;
  }
  std::vector<std::shared_ptr<IObject>> objects;
  objects.reserve(triangles_.size());
  for (const auto& t : triangles_) {
    objects.push_back(std::static_pointer_cast<IObject>(t));
  }
  internalBVH_ = std::make_shared<raytracer::components::BVHNode>(objects);
}
}  // namespace raytracer::components::primitives
