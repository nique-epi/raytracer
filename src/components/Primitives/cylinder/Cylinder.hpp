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
  std::shared_ptr<IMaterial> material_ = nullptr;

  /**
   * @brief Compute quadratic coefficients for ray-cylinder intersection.
   *
   * The cylinder surface equation is solved by projecting the ray onto the
   * plane perpendicular to the axis, reducing the problem to a 2-D circle
   * intersection. The resulting quadratic is:
   *   perpDirLengthSq·t² + 2·perpDirOriginDot·t + perpOriginSqMinusRadiusSq = 0
   *
   * @param [in]  ray                        The incoming ray.
   * @param [out] perpDirLengthSq            |D_perp|² — squared length of the
   *                                         direction component perpendicular
   *                                         to the cylinder axis.
   * @param [out] perpDirOriginDot           D_perp · w_perp — dot product of
   *                                         the perpendicular direction and the
   *                                         perpendicular origin offset.
   * @param [out] perpOriginSqMinusRadiusSq  |w_perp|² − r² — squared radial
   *                                         distance from the axis minus the
   *                                         squared radius.
   * @returns false if the ray is parallel to the cylinder axis (degenerate).
   */
  bool computeQuadraticCoeffs(const math::Ray& ray, double& perpDirLengthSq,
                              double& perpDirOriginDot,
                              double& perpOriginSqMinusRadiusSq) const;

  /**
   * @brief Find the smallest t in [tMin, tMax] that hits the cylinder body.
   *
   * @param [in] ray                        The incoming ray.
   * @param [in] tMin                       Lower bound for t.
   * @param [in] tMax                       Upper bound for t.
   * @param [in] perpDirLengthSq            |D_perp|² — see computeQuadraticCoeffs.
   * @param [in] perpDirOriginDot           D_perp · w_perp — see computeQuadraticCoeffs.
   * @param [in] perpOriginSqMinusRadiusSq  |w_perp|² − r² — see computeQuadraticCoeffs.
   * @returns The closest valid t, or -1.0 if no valid root exists.
   */
  double findClosestValidRoot(const math::Ray& ray, double tMin, double tMax,
                              double perpDirLengthSq, double perpDirOriginDot,
                              double perpOriginSqMinusRadiusSq) const;
};

}  // namespace raytracer::components::primitives
