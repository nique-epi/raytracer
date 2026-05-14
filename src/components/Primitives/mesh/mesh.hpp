/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** mesh
*/

#pragma once

#include <memory>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "components/Primitives/triangle/Triangle.hpp"

namespace raytracer::components {
class BVHNode;
}

namespace raytracer::components::primitives {
class Mesh : public IObject {
 public:
  Mesh() = default;
  explicit Mesh(std::vector<std::shared_ptr<Triangle>> triangles);
  ~Mesh() override = default;

  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;
  Mesh(Mesh&&) = delete;
  Mesh& operator=(Mesh&&) = delete;

  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

  void buildAccelerationStructure() override;

  void addTriangle(std::shared_ptr<Triangle> triangle);

 private:
  std::vector<std::shared_ptr<raytracer::components::primitives::Triangle>>
      triangles_;
  mutable bool bboxDirty_ = true;
  mutable raytracer::math::AABB cachedBox_;
  std::shared_ptr<raytracer::components::BVHNode> internalBVH_;
};
}  // namespace raytracer::components::primitives
