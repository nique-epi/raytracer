/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** Collection (composite) of IObject
*/

#ifndef PRIMITIVE_COLLECTION_HPP_
#define PRIMITIVE_COLLECTION_HPP_

#include <memory>
#include <vector>
#include "IObject.hpp"

namespace raytracer::components {

/**
 * @brief Composite collection implementing the Composite pattern over IObject.
 *
 * Exposes the same interface as its children, allowing a scene to be treated
 * as a single hittable object. Maintains a dynamic AABB that is recalculated
 * whenever objects are added or transformations are applied.
 */
class Collection : public IObject {
 public:
  Collection() = default;
  ~Collection() override = default;

  Collection(const Collection&) = delete;
  Collection& operator=(const Collection&) = delete;
  Collection(Collection&&) = delete;
  Collection& operator=(Collection&&) = delete;

  /**
   * @brief Add a child object to this collection.
   *
   * Updates the bounding box using AABB::surrounding() to encompass all
   * children.
   *
   * @param [in] obj  The object to add. If nullptr, the call is a no-op.
   */
  void add(const std::shared_ptr<IObject>& obj);

  /**
   * @brief Test whether the ray hits any child object in [tMin, tMax].
   *
   * Iterates through all children and records the closest intersection,
   * advancing tMax for each hit to ensure only the nearest object is returned.
   *
   * @param [in]  ray   The incoming ray.
   * @param [in]  tMin  Minimum t value (inclusive).
   * @param [in]  tMax  Maximum t value (inclusive).
   * @param [out] rec   Filled with the closest intersection data if and only if
   *                    @c true is returned; left unmodified otherwise.
   * @returns true  if an intersection exists within [tMin, tMax].
   * @returns false otherwise.
   */
  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  /**
   * @brief Return the axis-aligned bounding box encompassing all children.
   *
   * @returns The computed AABB that bounds all child objects.
   */
  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  /**
   * @brief Apply a transformation to all child objects in-place.
   *
   * After applying the transformation to each child, the collection's bounding
   * box is recalculated to reflect the new positions.
   *
   * @param [in] transform  The transformation to apply.
   */
  void applyTransformation(const ITransformation& transform) override;

 private:
  std::vector<std::shared_ptr<IObject>> children_;
  raytracer::math::AABB boundingBox_;
};

}  // namespace raytracer::components

#endif  // PRIMITIVE_COLLECTION_HPP_
