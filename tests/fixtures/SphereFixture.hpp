/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SphereFixture fixture for unit tests
*/

#ifndef TESTS_FIXTURES_SPHEREFIXTURE_HPP_
#define TESTS_FIXTURES_SPHEREFIXTURE_HPP_

#include "components/Primitives/IObject.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"
#include "utils/math/Vector3D.hpp"

// Minimal test sphere implementing IObject for unit testing
class SphereFixture : public IObject {
 public:
  SphereFixture(const raytracer::math::Vector3D& c, double r);

  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

 private:
  raytracer::math::Vector3D center_;
  double radius_;
};

#endif  // TESTS_FIXTURES_SPHEREFIXTURE_HPP_
