/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#include "components/Primitives/BVH/BVHNode.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "components/Transformations/ITransformation.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components {

BVHNode::BVHNode(std::vector<std::shared_ptr<IObject>> objects)
    : leftChild_(nullptr), rightChild_(nullptr) {
  build(objects, 0, objects.size());
}

// NOLINTNEXTLINE(misc-no-recursion)
BVHNode::BVHNode(RangeTag tag, std::vector<std::shared_ptr<IObject>>& objects,
                 std::size_t start, std::size_t end)
    : leftChild_(nullptr), rightChild_(nullptr) {
  (void)tag;
  build(objects, start, end);
}

// NOLINTNEXTLINE(misc-no-recursion)
void BVHNode::build(std::vector<std::shared_ptr<IObject>>& objects,
                    std::size_t start, std::size_t end) {
  const std::size_t count = end - start;
  if (count == 0) {
    return;
  }
  if (count == 1) {
    leftChild_ = objects[start];
    rightChild_ = nullptr;
    boundingBox_ = leftChild_->getBoundingBox();
    return;
  }
  if (count == 2) {
    leftChild_ = objects[start];
    rightChild_ = objects[start + 1];
    boundingBox_ = raytracer::math::AABB::surrounding(
        leftChild_->getBoundingBox(), rightChild_->getBoundingBox());
    return;
  }

  const int splitAxis = computeSplitAxis(objects, start, end);
  sortByCentroid(objects, start, end, splitAxis);

  const std::size_t mid = (start + end) / 2;
  leftChild_ = std::make_shared<BVHNode>(RangeTag{}, objects, start, mid);
  rightChild_ = std::make_shared<BVHNode>(RangeTag{}, objects, mid, end);
  boundingBox_ = raytracer::math::AABB::surrounding(
      leftChild_->getBoundingBox(), rightChild_->getBoundingBox());
}

int BVHNode::computeSplitAxis(
    const std::vector<std::shared_ptr<IObject>>& objects, std::size_t start,
    std::size_t end) {
  raytracer::math::AABB encompassing = objects[start]->getBoundingBox();
  // NOLINTNEXTLINE(altera-id-dependent-backward-branch)
  for (std::size_t index = start + 1; index < end; ++index) {
    encompassing = raytracer::math::AABB::surrounding(
        encompassing, objects[index]->getBoundingBox());
  }
  return longestAxis(encompassing);
}

void BVHNode::sortByCentroid(std::vector<std::shared_ptr<IObject>>& objects,
                             std::size_t start, std::size_t end, int axis) {
  double raytracer::math::Vector3D::* component = &raytracer::math::Vector3D::z;
  if (axis == 0) {
    component = &raytracer::math::Vector3D::x;
  } else if (axis == 1) {
    component = &raytracer::math::Vector3D::y;
  }

  const auto comparator = [component](const std::shared_ptr<IObject>& objectA,
                                      const std::shared_ptr<IObject>& objectB) {
    const raytracer::math::AABB boxA = objectA->getBoundingBox();
    const raytracer::math::AABB boxB = objectB->getBoundingBox();
    return safeCentroid(boxA.min.*component, boxA.max.*component) <
           safeCentroid(boxB.min.*component, boxB.max.*component);
  };

  const auto beginIter = objects.begin() + static_cast<std::ptrdiff_t>(start);
  const auto endIter = objects.begin() + static_cast<std::ptrdiff_t>(end);
  std::sort(beginIter, endIter, comparator);
}

bool BVHNode::hits(const raytracer::math::Ray& ray, double tMin, double tMax,
                   raytracer::math::HitRecord& rec) const {
  if (!boundingBox_.hit(ray, tMin, tMax)) {
    return false;
  }

  const bool hitLeft =
      leftChild_ != nullptr && leftChild_->hits(ray, tMin, tMax, rec);
  const double closestSoFar = hitLeft ? rec.t : tMax;
  const bool hitRight =
      rightChild_ != nullptr && rightChild_->hits(ray, tMin, closestSoFar, rec);

  return hitLeft || hitRight;
}

raytracer::math::AABB BVHNode::getBoundingBox() const { return boundingBox_; }

void BVHNode::applyTransformation(const ITransformation& transform) {
  if (leftChild_ != nullptr) {
    leftChild_->applyTransformation(transform);
  }
  if (rightChild_ != nullptr) {
    rightChild_->applyTransformation(transform);
  }
  if (leftChild_ != nullptr && rightChild_ != nullptr) {
    boundingBox_ = raytracer::math::AABB::surrounding(
        leftChild_->getBoundingBox(), rightChild_->getBoundingBox());
  } else if (leftChild_ != nullptr) {
    boundingBox_ = leftChild_->getBoundingBox();
  }
}

int BVHNode::longestAxis(const raytracer::math::AABB& box) {
  const double xExtent = box.max.x - box.min.x;
  const double yExtent = box.max.y - box.min.y;
  const double zExtent = box.max.z - box.min.z;
  if (xExtent > yExtent && xExtent > zExtent) {
    return 0;
  }
  if (yExtent > zExtent) {
    return 1;
  }
  return 2;
}

double BVHNode::safeCentroid(double minVal, double maxVal) {
  const double centroid = (minVal + maxVal) / 2.0;
  if (std::isinf(centroid) || std::isnan(centroid)) {
    return 0.0;
  }
  return centroid;
}

}  // namespace raytracer::components
