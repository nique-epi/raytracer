/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** HitRecord
*/

#ifndef MATH_HITRECORD_HPP_
#define MATH_HITRECORD_HPP_

#include <memory>
#include "Ray.hpp"
#include "Vector3D.hpp"

class IMaterial;

namespace Math {

class HitRecord {
 public:
  Vector3D point;
  Vector3D normal;
  double t{0.0};
  bool frontFace{false};
  double u{0.0};
  double v{0.0};
  std::shared_ptr<IMaterial> material;

  void setFaceNormal(const Ray& ray, const Vector3D& outNormal);
};

}  // namespace Math

#endif  // MATH_HITRECORD_HPP_
