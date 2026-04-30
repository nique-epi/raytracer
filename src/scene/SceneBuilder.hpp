/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneBuilder
*/

#pragma once
#include <libconfig.h++>
#include <string>
#include <vector>

namespace raytracer::scene {

/**
 * @brief Stub builder used by scene loaders to register parsed entities.
 *
 * @note This is a temporary stub: the production builder lives in NIQ-125
 *       (#19) and depends on Registry<T> (#11) and the Object/ILight/ICamera
 *       interfaces. The stub records only the type discriminant of each
 *       call, which is enough to test loader behaviour but not enough to
 *       instantiate concrete primitives. The `cfg` parameter shape is fixed
 *       by the issue spec to avoid churn in the loaders when the real
 *       implementation lands.
 */
class SceneBuilder {
 public:
  /**
   * @brief Single entry recorded by the stub for each addObject() call.
   */
  struct Entry {
    /** Type discriminant (e.g. "sphere", "plane", "ambient"). */
    std::string type;
  };

  /**
   * @brief Register one parsed entity into the builder.
   *
   * The real implementation (NIQ-125) will resolve @p type via the matching
   * Registry and instantiate a concrete object from @p cfg. This stub only
   * records @p type; @p cfg is intentionally ignored.
   *
   * @param [in] type Type discriminant chosen by the loader.
   * @param [in] cfg  Configuration block describing the entity (unused in
   *                  this stub, kept for API stability with NIQ-125).
   */
  void addObject(const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Access every entry recorded so far in insertion order.
   *
   * @returns Const reference to the internal entries vector.
   */
  [[nodiscard]] const std::vector<Entry>& entries() const;

  /**
   * @brief Count how many entries match a given type discriminant.
   *
   * @param [in] type Type discriminant to match.
   * @returns Number of recorded entries whose `type` equals @p type.
   */
  [[nodiscard]] std::size_t count(const std::string& type) const;

 private:
  std::vector<Entry> _entries;
};

}  // namespace raytracer::scene
