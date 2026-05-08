/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Cylinder
*/

#pragma once

#include <memory>
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

class IMaterial;

namespace raytracer::components::primitives {

class Cylinder : public IObject {
 public:
  Cylinder(const math::Vector3D& center, const math::Vector3D& axis,
           double radius, double height, std::shared_ptr<IMaterial> material);

  bool hits(const math::Ray& ray, double tMin, double tMax,
            math::HitRecord& rec) const override;

  [[nodiscard]] math::AABB getBoundingBox() const override;

  void applyTransformation(const ITransformation& transform) override;

 private:
  math::Vector3D center_;
  math::Vector3D axis_;
  double radius_;
  double height_;
  std::shared_ptr<IMaterial> material_;

  /**
   * @brief Compute quadratic coefficients for ray-cylinder intersection.
   *
   * @param [in]  ray    The incoming ray.
   * @param [out] a      Coefficient of t².
   * @param [out] halfB  Half-coefficient of t (half-b convention).
   * @param [out] c      Constant coefficient.
   * @returns false if the ray is parallel to the cylinder axis (degenerate).
   */
  bool computeQuadraticCoeffs(const math::Ray& ray, double& a, double& halfB,
                              double& c) const;

  /**
   * @brief Find the smallest t in [tMin, tMax] that hits the cylinder body.
   *
   * @param [in] ray    The incoming ray.
   * @param [in] tMin   Lower bound for t.
   * @param [in] tMax   Upper bound for t.
   * @param [in] a      Coefficient of t².
   * @param [in] halfB  Half-coefficient of t.
   * @param [in] c      Constant coefficient.
   * @returns The closest valid t, or -1.0 if no valid root exists.
   */
  double findClosestValidRoot(const math::Ray& ray, double tMin, double tMax,
                              double a, double halfB, double c) const;
};

}  // namespace raytracer::components::primitives
