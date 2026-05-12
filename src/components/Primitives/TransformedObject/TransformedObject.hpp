/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformedObject
*/

#pragma once

#include <memory>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "components/Transformations/ITransformation.hpp"

/**
 * @brief Decorate an object with a stack of transformations.
 *
 * This class stores a primitive object and a list of transformations applied
 * to it without modifying the primitive itself. Transformations are stacked in
 * insertion order and are meant to be composed when testing intersections or
 * computing the bounding box.
 */

namespace raytracer::components::primitives {

class TransformedObject : public IObject {
 public:
  /**
   * @brief Construct an empty transformed object.
   */
  TransformedObject();

  /**
   * @brief Destroy this transformed object.
   */
  ~TransformedObject() override = default;

  TransformedObject(const TransformedObject&) = delete;
  TransformedObject& operator=(const TransformedObject&) = delete;
  TransformedObject(TransformedObject&&) = delete;
  TransformedObject& operator=(TransformedObject&&) = delete;

  /**
   * @brief Set the primitive object wrapped by this decorator.
   *
   * @param [in] primitive The object to decorate.
   */
  void setPrimitive(const std::shared_ptr<IObject>& primitive);

  /**
   * @brief Add a transformation to the stack.
   *
   * Transformations are stored in insertion order and applied as a composed
   * stack when evaluating hits or the bounding box.
   *
   * @param [in] transform The transformation to append.
   */
  void addTransform(const std::shared_ptr<ITransformation>& transform);

  /**
   * @brief Test whether the transformed primitive is hit by a ray.
   *
   * The incoming ray is converted to the local space of the primitive using
   * the inverse composed transformation before delegating the intersection
   * test to the wrapped primitive.
   *
   * @param [in]  ray   The incoming ray in world space.
   * @param [in]  tMin  Minimum t value (inclusive).
   * @param [in]  tMax  Maximum t value (inclusive).
   * @param [out] rec   Filled with intersection data if and only if @c true is
   *                    returned; left unmodified otherwise.
   * @returns true if an intersection exists within [tMin, tMax].
   * @returns false otherwise.
   */
  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  /**
   * @brief Return the transformed axis-aligned bounding box.
   *
   * The local bounding box of the primitive is transformed through the direct
   * composed transformation by evaluating its eight corners.
   *
   * @returns The bounding box of the transformed object.
   */
  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  /**
   * @brief Apply a transformation to the wrapped primitive.
   *
   * @param [in] transform The transformation to apply.
   */
  void applyTransformation(const ITransformation& transform) override;

 private:
  std::shared_ptr<IObject> primitive_;
  std::vector<std::shared_ptr<ITransformation>> transforms_;
};  // namespace classTransformedObject:public IObject
}  // namespace raytracer::components::primitives
