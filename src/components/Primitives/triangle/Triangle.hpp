/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#pragma once

#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

namespace raytracer::components::primitives {

class Triangle : public IObject {
 public:
  Triangle();
  Triangle(const math::Point3D& vertex0, const math::Point3D& vertex1,
           const math::Point3D& vertex2, std::shared_ptr<IMaterial> material);
  ~Triangle() override;

  Triangle(const Triangle&) = delete;
  Triangle& operator=(const Triangle&) = delete;
  Triangle(Triangle&&) = delete;
  Triangle& operator=(Triangle&&) = delete;

  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

 private:
  std::shared_ptr<IMaterial> material_;
  raytracer::math::Point3D vertex0_, vertex1_, vertex2_;
};

}  // namespace raytracer::components::primitives
