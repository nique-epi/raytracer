/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** HitRecord
*/

#include "HitRecord.hpp"

namespace raytracer::math {

void HitRecord::setFaceNormal(const Ray& ray, const Vector3D& outNormal) {
  frontFace = ray.getDirection().dot(outNormal) < 0.0;
  normal = frontFace ? outNormal : -outNormal;
}

}  // namespace raytracer::math
