/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** AABB
*/

#ifndef MATH_AABB_HPP_
#define MATH_AABB_HPP_

#include "Math/Ray.hpp"
#include "Math/Vector3D.hpp"

namespace Math {

class AABB {
 public:
  Vector3D min;
  Vector3D max;

  AABB();
  AABB(const Vector3D& min_, const Vector3D& max_);

  AABB(const AABB&) = default;
  AABB& operator=(const AABB&) = default;
  AABB(AABB&&) = default;
  AABB& operator=(AABB&&) = default;
  ~AABB() = default;

  [[nodiscard]] bool hit(const Ray& ray, double tMin, double tMax) const;
  [[nodiscard]] bool contains(const Vector3D& point) const;

  [[nodiscard]] static AABB surrounding(const AABB& a, const AABB& b);
};

}  // namespace Math

#endif  // MATH_AABB_HPP_
