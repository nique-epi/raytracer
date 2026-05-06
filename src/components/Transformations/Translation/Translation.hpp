/*
 ** EPITECH PROJECT, 2026
 ** raytracer
 ** File description:
 ** Translation
 */

#pragma once

#include "components/Transformations/ITransformation.hpp"

/**
 * @brief Translation transformation for moving points and normals in space.
 *
 * Applies a uniform offset to points and normals. The translation is
 * represented by a 3D vector that specifies the displacement in x, y, and z
 * directions.
 *
 * @note Translations commute with each other and with rotations about the
 *       origin. However, translations do not commute with rotations about
 *       arbitrary points.
 */
namespace raytracer::components::transformation {

/**
 * @brief A transformation that translates points by a fixed offset.
 *
 * Translation shifts all points and normals by the same offset vector.
 * This transformation is commonly used to position objects in the scene.
 *
 * @see ITransformation for the interface contract.
 */
class Translation : public ITransformation {
 public:
  /**
   * @brief Construct a default translation.
   *
   * Creates a translation with zero offset (identity transformation).
   */
  Translation() = default;

  /**
   * @brief Construct a translation with a specific offset.
   *
   * @param [in] offset The displacement vector to apply to all points.
   */
  explicit Translation(const raytracer::math::Vector3D& offset);

  /**
   * @brief Destructor.
   */
  ~Translation() override;

  /**
   * @brief Copy constructor.
   *
   * @param [in] other The source translation to copy.
   */
  Translation(const Translation& other) = default;

  /**
   * @brief Copy assignment operator.
   *
   * @param [in] other The source translation to copy from.
   * @returns Reference to this translation.
   */
  Translation& operator=(const Translation& other) = default;

  /**
   * @brief Move constructor.
   *
   * @param [in] other The source translation to move from.
   * @returns Reference to this translation.
   */
  Translation(Translation&& other) noexcept = default;

  /**
   * @brief Move assignment operator.
   *
   * @param [in] other The source translation to move from.
   * @returns Reference to this translation.
   */
  Translation& operator=(Translation&& other) noexcept = default;

  /**
   * @brief Set the translation offset.
   *
   * @param [in] offset The new displacement vector.
   */
  void setOffset(const raytracer::math::Vector3D& offset);

  /**
   * @brief Apply the translation to a point.
   *
   * Computes the new position of a point after applying the translation.
   *
   * @param [in] point The point to translate.
   * @returns The translated point.
   */
  [[nodiscard]] raytracer::math::Vector3D apply(
      const raytracer::math::Vector3D& point) const override;

  /**
   * @brief Apply the translation to a normal vector.
   *
   * Translates a normal vector by the same offset as points.
   *
   * @param [in] normal The normal to translate.
   * @returns The translated normal.
   */
  [[nodiscard]] raytracer::math::Vector3D applyToNormal(
      const raytracer::math::Vector3D& normal) const override;

  /**
   * @brief Compute the inverse transformation.
   *
   * Returns a translation with the negated offset.
   *
   * @returns A shared pointer to a new Translation with negated offset.
   */
  [[nodiscard]] std::shared_ptr<ITransformation> inverse() const override;

 private:
  raytracer::math::Vector3D offset_;
};

}  // namespace raytracer::components::transformation
