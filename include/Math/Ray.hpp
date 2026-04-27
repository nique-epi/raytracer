/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Ray
*/

#ifndef MATH_RAY_HPP_
#define MATH_RAY_HPP_

#include "Math/Vector3D.hpp"

namespace Math {

class Ray {
 public:
  Ray(const Vector3D& origin, const Vector3D& direction);

  Ray(const Ray&) = default;
  Ray& operator=(const Ray&) = default;
  Ray(Ray&&) = default;
  Ray& operator=(Ray&&) = default;
  ~Ray() = default;

  [[nodiscard]] const Vector3D& getOrigin() const;
  [[nodiscard]] const Vector3D& getDirection() const;
  [[nodiscard]] Vector3D at(double t) const;

 private:
  Vector3D _origin;
  Vector3D _direction;
};

}  // namespace Math

#endif  // MATH_RAY_HPP_
