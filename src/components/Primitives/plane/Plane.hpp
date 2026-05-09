/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Plane
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

class IMaterial;

namespace raytracer::components::primitives {

/**
 * @brief Infinite plane primitive defined by a point and a normal.
 */
class Plane : public IObject {
 public:
  Plane();
  Plane(const math::Vector3D& point, const math::Vector3D& normal,
        std::shared_ptr<IMaterial> material);

  bool hits(const math::Ray& ray, double tMin, double tMax,
            math::HitRecord& rec) const override;

  [[nodiscard]] math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

  void setPoint(const math::Vector3D& point);
  void setNormal(const math::Vector3D& normal);
  void setMaterial(std::shared_ptr<IMaterial> material);

 private:
  math::Vector3D point_;
  math::Vector3D normal_;
  std::shared_ptr<IMaterial> material_;
};

}  // namespace raytracer::components::primitives
