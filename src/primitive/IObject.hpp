/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** IObject
*/

#ifndef PRIMITIVE_IOBJECT_HPP_
#define PRIMITIVE_IOBJECT_HPP_

#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

class ITransformation;

/**
 * @brief Contract shared by all scene objects (primitives, collections,
 *        transformed objects, meshes).
 *
 * Conventions:
 *  - @p tMin and @p tMax are **inclusive** bounds on the ray parameter t.
 *  - @p rec is written only when the method returns @c true.
 */
class IObject {
 public:
  IObject() = default;
  virtual ~IObject() = default;

  IObject(const IObject&) = delete;
  IObject& operator=(const IObject&) = delete;
  IObject(IObject&&) = delete;
  IObject& operator=(IObject&&) = delete;

  /**
   * @brief Test whether the ray hits this object in [tMin, tMax].
   *
   * @param [in]  ray   The incoming ray.
   * @param [in]  tMin  Minimum t value (inclusive).
   * @param [in]  tMax  Maximum t value (inclusive).
   * @param [out] rec   Filled with intersection data if and only if @c true is
   *                    returned; left unmodified otherwise.
   * @returns true  if an intersection exists within [tMin, tMax].
   * @returns false otherwise.
   */
  virtual bool hits(const Math::Ray& ray, double tMin, double tMax,
                    Math::HitRecord& rec) const = 0;

  /**
   * @brief Return the axis-aligned bounding box of this object.
   */
  [[nodiscard]] virtual Math::AABB getBoundingBox() const = 0;

  /**
   * @brief Apply a transformation to this object in-place.
   */
  virtual void applyTransformation(const ITransformation& transform) = 0;
};

#endif  // PRIMITIVE_IOBJECT_HPP_
