/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Triangle
*/

#pragma once

#include "components/Primitives/IObject.hpp"

namespace raytracer::primitives {

class Triangle : public IObject {
 public:
  Triangle();
  ~Triangle() override;

  Triangle(const Triangle&) = delete;
  Triangle& operator=(const Triangle&) = delete;
  Triangle(Triangle&&) = delete;
  Triangle& operator=(Triangle&&) = delete;

  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;
};

}  // namespace raytracer::primitives
