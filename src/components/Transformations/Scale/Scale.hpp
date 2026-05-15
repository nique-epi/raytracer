/*
 ** EPITECH PROJECT, 2026
 ** raytracer
 ** File description:
 ** Scale
 */

#pragma once

#include "components/Transformations/ITransformation.hpp"

namespace raytracer::components::transformation {

/**
 * @brief Non-uniform scale transformation for resizing points and normals.
 *
 * Applies an independent scale factor along each axis.  This lets a sphere
 * become an ellipsoid, a cube become a rectangular box, etc.  The normal
 * transform uses the transpose-inverse rule so that surface normals remain
 * perpendicular to their geometric surface after scaling.
 *
 * @see ITransformation for the interface contract.
 * @see Translation for complementary positional transformations.
 */
class Scale : public ITransformation {
 public:
  /**
   * @brief Construct a default scale (identity: factor = (1, 1, 1)).
   */
  Scale() = default;

  /**
   * @brief Construct a scale with specific per-axis factors.
   *
   * @param [in] factor Scale factors along x, y and z.  Any component equal
   *             to zero is treated as a degenerate scale; @p inverse() will
   *             clamp it to a small epsilon rather than dividing by zero.
   */
  explicit Scale(const raytracer::math::Vector3D& factor);

  /**
   * @brief Destructor.
   */
  ~Scale() override = default;

  /**
   * @brief Copy constructor.
   *
   * @param [in] other The source scale to copy.
   */
  Scale(const Scale& other) = default;

  /**
   * @brief Copy assignment operator.
   *
   * @param [in] other The source scale to copy from.
   * @returns Reference to this scale.
   */
  Scale& operator=(const Scale& other) = default;

  /**
   * @brief Move constructor.
   *
   * @param [in] other The source scale to move from.
   */
  Scale(Scale&& other) noexcept = default;

  /**
   * @brief Move assignment operator.
   *
   * @param [in] other The source scale to move from.
   * @returns Reference to this scale.
   */
  Scale& operator=(Scale&& other) noexcept = default;

  /**
   * @brief Set the scale factor.
   *
   * @param [in] factor The new per-axis scale factors.
   */
  void setFactor(const raytracer::math::Vector3D& factor);

  /**
   * @brief Apply the scale to a point.
   *
   * @param [in] point The point to scale.
   * @returns The scaled point: (p.x*fx, p.y*fy, p.z*fz).
   */
  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;

  /**
   * @brief Apply the scale to a surface normal.
   *
   * Uses the transpose-inverse rule: divides each component by its factor,
   * then normalizes so the result is a unit normal.
   *
   * @param [in] normal The normal to transform.
   * @returns The transformed, normalized normal.
   */
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;

  /**
   * @brief Compute the inverse transformation.
   *
   * Returns a Scale with factor (1/fx, 1/fy, 1/fz).  Components whose
   * absolute value is below epsilon are clamped to epsilon to avoid
   * division by zero.
   *
   * @returns A shared pointer to the inverse Scale.
   */
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  raytracer::math::Vector3D factor_{1.0, 1.0, 1.0};
};

}  // namespace raytracer::components::transformation
