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
  Cone();
  Cone(const math::Vector3D& apex, const math::Vector3D& axis, double angle,
       double height, std::shared_ptr<IMaterial> material);

  bool hits(const math::Ray& ray, double tMin, double tMax,
            math::HitRecord& rec) const override;

  [[nodiscard]] math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

  void setApex(const math::Vector3D& apex);
  void setAxis(const math::Vector3D& axis);
  void setHalfAngle(double angle);
  void setHeight(double height);
  void setMaterial(std::shared_ptr<IMaterial> material);

 private:
  math::Vector3D apex_;
  math::Vector3D axis_;
  double angle_;
  double height_;
  double cosAngle_;
  double sinAngle_;
  std::shared_ptr<IMaterial> material_;

  /**
   * @brief Compute the quadratic coefficients for the ray-cone intersection.
   *
   * @param [in]  ray       The incoming ray.
   * @param [out] quadA     Coefficient of t².
   * @param [out] quadHalfB Half-coefficient of t (uses half-b convention).
   * @param [out] quadC     Constant coefficient.
   * @returns false if the equation degenerates (ray parallel to cone surface).
   */
  bool computeQuadraticCoeffs(const math::Ray& ray, double& quadA,
                               double& quadHalfB, double& quadC) const;

  /**
   * @brief Find the smallest t in [tMin, tMax] that hits the finite cone.
   *
   * @returns The closest valid t, or -1.0 if no valid root exists.
   */
  double findClosestValidRoot(const math::Ray& ray, double tMin, double tMax,
                               double quadA, double quadHalfB,
                               double quadC) const;

  /**
   * @brief Compute the outward surface normal at a hit point.
   *
   * @param [in]  hitPoint  The intersection point on the cone surface.
   * @param [out] outNormal Filled with the outward normal if @c true is
   *                        returned.
   * @returns false if the hit point is at the apex (degenerate normal).
   */
  bool computeOutwardNormal(const math::Vector3D& hitPoint,
                             math::Vector3D& outNormal) const;
};

}  // namespace raytracer::components::primitives
