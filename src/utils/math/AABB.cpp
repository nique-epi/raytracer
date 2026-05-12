/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AABB
*/

#include "AABB.hpp"
#include <algorithm>
#include <array>
#include "Ray.hpp"

namespace raytracer::math {

AABB::AABB() : min(0.0, 0.0, 0.0), max(0.0, 0.0, 0.0) {}

AABB::AABB(const Vector3D& min_, const Vector3D& max_) : min(min_), max(max_) {}

bool AABB::hit(const Ray& ray, double tMin, double tMax) const {
  const Vector3D& orig = ray.getOrigin();
  const Vector3D& dir = ray.getDirection();

  const std::array<double, 3> orig_a = {orig.x, orig.y, orig.z};
  const std::array<double, 3> dir_a = {dir.x, dir.y, dir.z};
  const std::array<double, 3> min_a = {min.x, min.y, min.z};
  const std::array<double, 3> max_a = {max.x, max.y, max.z};

  for (int i = 0; i < 3; ++i) {
    if (dir_a[i] == 0.0) {
      if (orig_a[i] < min_a[i] || orig_a[i] >= max_a[i]) {
        return false;
      }
      continue;
    }

    const double invD = 1.0 / dir_a[i];
    double t0 = (min_a[i] - orig_a[i]) * invD;
    double t1 = (max_a[i] - orig_a[i]) * invD;

    if (invD < 0.0) {
      std::swap(t0, t1);
    }

    tMin = std::max(t0, tMin);
    tMax = std::min(t1, tMax);

    if (tMax < tMin) {
      return false;
    }
  }
  return true;
}

bool AABB::contains(const Vector3D& point) const {
  return point.x >= min.x && point.x <= max.x && point.y >= min.y &&
         point.y <= max.y && point.z >= min.z && point.z <= max.z;
}

AABB AABB::surrounding(const AABB& a, const AABB& b) {
  const Vector3D small(std::min(a.min.x, b.min.x), std::min(a.min.y, b.min.y),
                       std::min(a.min.z, b.min.z));
  const Vector3D big(std::max(a.max.x, b.max.x), std::max(a.max.y, b.max.y),
                     std::max(a.max.z, b.max.z));
  return {small, big};
}

}  // namespace raytracer::math
