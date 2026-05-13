/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Collection (composite) of IObject
*/

#include "components/Primitives/Collection/Collection.hpp"

namespace raytracer::components {

void Collection::add(const std::shared_ptr<IObject>& obj) {
  if (!obj) {
    return;
  }
  children_.push_back(obj);
  if (children_.size() == 1) {
    boundingBox_ = obj->getBoundingBox();
  } else {
    boundingBox_ =
        raytracer::math::AABB::surrounding(boundingBox_, obj->getBoundingBox());
  }
}

bool Collection::hits(const raytracer::math::Ray& ray, double tMin, double tMax,
                      raytracer::math::HitRecord& rec) const {
  bool hitAnything = false;
  double closestSoFar = tMax;
  raytracer::math::HitRecord tempRec;

  for (const auto& child : children_) {
    if (!child) {
      continue;
    }
    if (child->hits(ray, tMin, closestSoFar, tempRec)) {
      hitAnything = true;
      closestSoFar = tempRec.t;
      rec = tempRec;
    }
  }
  return hitAnything;
}

raytracer::math::AABB Collection::getBoundingBox() const {
  return boundingBox_;
}

const std::vector<std::shared_ptr<IObject>>& Collection::getObjects() const {
  return children_;
}

void Collection::applyTransformation(const ITransformation& transform) {
  for (const auto& child : children_) {
    if (child) {
      child->applyTransformation(transform);
    }
  }

  if (children_.empty()) {
    boundingBox_ = raytracer::math::AABB();
    return;
  }
  boundingBox_ = children_.front()->getBoundingBox();
  for (size_t i = 1; i < children_.size(); ++i) {
    if (!children_[i]) {
      continue;
    }
    boundingBox_ = raytracer::math::AABB::surrounding(
        boundingBox_, children_[i]->getBoundingBox());
  }
}

}  // namespace raytracer::components
