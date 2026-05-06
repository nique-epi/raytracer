/*
 ** EPITECH PROJECT, 2026
 ** raytracer
 ** File description:
 ** Rotation
 */

#pragma once

#include "components/Transformations/ITransformation.hpp"
#include "utils/math/Matrix.hpp"

/**
 * @brief Rotation transformation for rotating points and normals around an
 * axis.
 *
 * Applies a rotation around a specified axis by a given angle. The rotation
 * is represented by a 4x4 matrix that transforms points and normals.
 *
 * @note Rotations preserve the length of vectors and angles between vectors.
 *       The rotation matrix is orthogonal with determinant +1.
 */
namespace raytracer::components::transformation {

/**
 * @brief A transformation that rotates points around an axis by a given angle.
 *
 * Rotation transforms points and normals around a specified axis (normalized)
 * by a given angle in radians. The rotation is represented internally by a
 * 4x4 matrix for efficient application to points and normals.
 *
 * @see ITransformation for the interface contract.
 * @see Translation for complementary translation transformations.
 */
class Rotation : public ITransformation {
 public:
  /**
   * @brief Construct a default rotation.
   *
   * Creates a rotation with no axis and zero angle (identity transformation).
   */
  Rotation();

  /**
   * @brief Construct a rotation with a specific axis and angle.
   *
   * @param [in] axis The normalized axis of rotation.
   * @param [in] angle The rotation angle in radians.
   */
  Rotation(const raytracer::math::Vector3D& axis, double angle);

  /**
   * @brief Destructor.
   */
  ~Rotation() override = default;

  /**
   * @brief Copy constructor.
   *
   * @param [in] other The source rotation to copy.
   */
  Rotation(const Rotation& other) = default;

  /**
   * @brief Copy assignment operator.
   *
   * @param [in] other The source rotation to copy from.
   * @returns Reference to this rotation.
   */
  Rotation& operator=(const Rotation& other) = default;

  /**
   * @brief Move constructor.
   *
   * @param [in] other The source rotation to move from.
   * @returns Reference to this rotation.
   */
  Rotation(Rotation&& other) noexcept = default;

  /**
   * @brief Move assignment operator.
   *
   * @param [in] other The source rotation to move from.
   * @returns Reference to this rotation.
   */
  Rotation& operator=(Rotation&& other) noexcept = default;

  /**
   * @brief Set the rotation axis.
   *
   * @param [in] axis The new axis of rotation (will be normalized).
   */
  void setAxis(const raytracer::math::Vector3D& axis);

  /**
   * @brief Set the rotation angle.
   *
   * @param [in] angle The new rotation angle in radians.
   */
  void setAngle(double angle);

  /**
   * @brief Set both axis and angle at once.
   *
   * @param [in] axis The axis of rotation (will be normalized).
   * @param [in] angle The rotation angle in radians.
   */
  void setRotation(const raytracer::math::Vector3D& axis, double angle);

  /**
   * @brief Apply the rotation to a point.
   *
   * Computes the new position of a point after applying the rotation.
   *
   * @param [in] point The point to rotate.
   * @returns The rotated point.
   */
  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;

  /**
   * @brief Apply the rotation to a normal vector.
   *
   * Rotates a normal vector by the same rotation as points.
   *
   * @param [in] normal The normal to rotate.
   * @returns The rotated normal.
   */
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;

  /**
   * @brief Compute the inverse transformation.
   *
   * Returns a rotation with the negated angle around the same axis.
   *
   * @returns A shared pointer to a new Rotation with negated angle.
   */
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  /**
   * @brief Synchronize the rotation matrix with the current axis and angle.
   *
   * Recomputes the rotation matrix whenever axis or angle changes.
   */
  void syncRotationMatrix();

  raytracer::math::Vector3D axis_;
  double angle_{0.0};
  raytracer::math::Matrix4x4 rotationMatrix_;
};

}  // namespace raytracer::components::transformation
