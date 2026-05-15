/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** BVHNode
*/

#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include "components/Primitives/IObject.hpp"
#include "components/Transformations/ITransformation.hpp"
#include "utils/math/AABB.hpp"
#include "utils/math/HitRecord.hpp"
#include "utils/math/Ray.hpp"

namespace raytracer::components {

/**
 * @brief Binary BVH node enabling O(log n) ray-scene intersection.
 *
 * Recursively partitions a list of objects along the longest AABB axis.
 * At traversal time the ray is tested against the node AABB first;
 * only on a hit are the children visited.
 */
class BVHNode : public IObject {
 public:
  /**
   * @brief Passkey for the internal range-based constructor.
   *
   * The type remains public so it can appear in the constructor
   * signature, but only BVHNode itself can create a RangeTag and
   * therefore call the internal range-based constructor.
   */
  struct RangeTag {
   private:
    friend class BVHNode;
    explicit RangeTag() = default;
  };

  /**
   * @brief Build a BVH tree from the given list of objects.
   *
   * @param [in] objects  Non-empty list of objects to organize.
   *                      Ownership is taken (passed by value).
   */
  explicit BVHNode(std::vector<std::shared_ptr<IObject>> objects);

  /**
   * @brief Internal range-based constructor used by std::make_shared.
   *
   * Prefer BVHNode(std::vector<...>) for external construction.
   *
   * @param [in]     tag      Passkey (use RangeTag{}).
   * @param [in,out] objects  Flat object list (may be sorted in-place).
   * @param [in]     start    Inclusive start index in @p objects.
   * @param [in]     end      Exclusive end index in @p objects.
   */
  BVHNode(RangeTag tag, std::vector<std::shared_ptr<IObject>>& objects,
          std::size_t start, std::size_t end);

  ~BVHNode() override = default;

  BVHNode(const BVHNode&) = delete;
  BVHNode& operator=(const BVHNode&) = delete;
  BVHNode(BVHNode&&) = delete;
  BVHNode& operator=(BVHNode&&) = delete;

  /**
   * @brief Test ray against this subtree; early-exit on bounding-box miss.
   *
   * @param [in]  ray   The incoming ray.
   * @param [in]  tMin  Minimum t value (inclusive).
   * @param [in]  tMax  Maximum t value (inclusive).
   * @param [out] rec   Closest intersection within [tMin, tMax].
   * @returns true  if any descendant object is hit within [tMin, tMax].
   * @returns false otherwise.
   */
  bool hits(const raytracer::math::Ray& ray, double tMin, double tMax,
            raytracer::math::HitRecord& rec) const override;

  /**
   * @brief Return the bounding box encompassing all objects in this subtree.
   */
  [[nodiscard]] raytracer::math::AABB getBoundingBox() const override;

  /**
   * @brief Apply a transformation to all objects and recompute bounding boxes.
   *
   * @param [in] transform  The transformation to apply.
   */
  void applyTransformation(const ITransformation& transform) override;

 private:
  void build(std::vector<std::shared_ptr<IObject>>& objects, std::size_t start,
             std::size_t end);

  static int computeSplitAxis(
      const std::vector<std::shared_ptr<IObject>>& objects, std::size_t start,
      std::size_t end);

  static void sortByCentroid(std::vector<std::shared_ptr<IObject>>& objects,
                             std::size_t start, std::size_t end, int axis);

  static int longestAxis(const raytracer::math::AABB& box);
  static double safeCentroid(double minVal, double maxVal);

  std::shared_ptr<IObject> leftChild_;
  std::shared_ptr<IObject> rightChild_;
  raytracer::math::AABB boundingBox_;
};

}  // namespace raytracer::components
