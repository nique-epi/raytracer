/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Sphere
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

class IMaterial;

namespace raytracer::components::primitives {

class Sphere : public IObject {
 public:
  Sphere();
  Sphere(const math::Vector3D& center, double radius,
         std::shared_ptr<IMaterial> material);

  bool hits(const math::Ray& ray, double tMin, double tMax,
            math::HitRecord& rec) const override;

  [[nodiscard]] math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

  void setCenter(const math::Vector3D& center);
  void setRadius(double radius);
  void setMaterial(std::shared_ptr<IMaterial> material);

 private:
  math::Vector3D center_;
  double radius_;
  std::shared_ptr<IMaterial> material_;

  static void computeUV(const math::Vector3D& unitSphere, double& u, double& v);
};

}  // namespace raytracer::components::primitives
