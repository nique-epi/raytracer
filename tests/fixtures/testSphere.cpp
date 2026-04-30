/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TestSphere fixture for unit tests
*/

#include "testSphere.hpp"
#include <cmath>

TestSphere::TestSphere(const raytracer::math::Vector3D& c, double r)
    : center_(c), radius_(r) {}

bool TestSphere::hits(const raytracer::math::Ray& ray, double tMin, double tMax,
                      raytracer::math::HitRecord& rec) const {
  const raytracer::math::Vector3D oc = ray.getOrigin() - center_;
  const raytracer::math::Vector3D dir = ray.getDirection();
  const double a = dir.dot(dir);
  const double b = 2.0 * oc.dot(dir);
  const double c = oc.dot(oc) - radius_ * radius_;
  const double discriminant = b * b - 4 * a * c;
  if (discriminant < 0) {
    return false;
  }
  const double sqrtd = std::sqrt(discriminant);
  double root = (-b - sqrtd) / (2.0 * a);
  if (root < tMin || root > tMax) {
    root = (-b + sqrtd) / (2.0 * a);
    if (root < tMin || root > tMax) {
      return false;
    }
  }
  rec.t = root;
  rec.point = ray.at(rec.t);
  rec.material = nullptr;
  // normal
  rec.normal = (rec.point - center_) / radius_;
  rec.setFaceNormal(ray, rec.normal);
  return true;
}

raytracer::math::AABB TestSphere::getBoundingBox() const {
  raytracer::math::Vector3D r(radius_, radius_, radius_);
  return raytracer::math::AABB(center_ - r, center_ + r);
}

void TestSphere::applyTransformation(const ITransformation&) {
  /* noop for test */
}
