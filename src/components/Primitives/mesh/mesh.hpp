/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** mesh
*/

#pragma once

#include <vector>
#include "components/Primitives/IObject.hpp"
#include "components/Primitives/triangle/Triangle.hpp"

namespace raytracer::components::primitives {
class Mesh : public IObject {
 public:
  Mesh() = default;
  explicit Mesh(const std::vector<Triangle>& triangles)
      : triangles_(triangles) {}
  ~Mesh() override = default;

  Mesh(const Mesh&);
  Mesh& operator=(const Mesh&);
  Mesh(Mesh&&) noexcept;
  Mesh& operator=(Mesh&&) noexcept;

  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

  void addTriangle(const Triangle& triangle) {
    triangles_.push_back(triangle);
    bboxDirty_ = true;
  }

 private:
  std::vector<raytracer::components::primitives::Triangle> triangles_;
  mutable bool bboxDirty_ = true;
  mutable raytracer::math::AABB cachedBox_;
};
}  // namespace raytracer::components::primitives
