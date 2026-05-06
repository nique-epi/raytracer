/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cone
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

class IMaterial;

namespace raytracer::components::primitives {

class Cone : public IObject {
 public:
  Cone(const math::Vector3D& apex, const math::Vector3D& axis, double angle,
       double height, std::shared_ptr<IMaterial> material);

  bool hits(const math::Ray& ray, double tMin, double tMax,
            math::HitRecord& rec) const override;

  [[nodiscard]] math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

 private:
  math::Vector3D apex_;
  math::Vector3D axis_;
  double angle_;
  double height_;
  double cosAngle_;
  double sinAngle_;
  std::shared_ptr<IMaterial> material_;
};

}  // namespace raytracer::components::primitives
