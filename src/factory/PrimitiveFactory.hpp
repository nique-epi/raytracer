/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** PrimitiveFactory
*/

#ifndef FACTORY_PRIMITIVEFACTORY_HPP_
#define FACTORY_PRIMITIVEFACTORY_HPP_

#include <memory>
#include <numbers>
#include <string>
#include "components/Primitives/IObject.hpp"
#include "utils/math/Vector3D.hpp"

class IMaterial;

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
inline constexpr double defaultConeAngle = std::numbers::pi / 4.0;

/**
 * @brief Static, statically-linked factory for `IObject` primitives.
 *
 * Exposes a typed creator per concrete primitive (sphere, cone, cylinder,
 * plane) plus a string-based `create(type, cfg)` dispatch that parses a
 * libconfig setting into the matching typed arguments and delegates to
 * the typed creator.
 *
 * Each typed creator carries its own default arguments; the string
 * dispatcher seeds local variables with the same defaults and only
 * overrides those actually present in @p cfg, so the "config is fallback"
 * contract is satisfied for both call paths.
 */
class PrimitiveFactory {
 public:
  PrimitiveFactory() = default;
  ~PrimitiveFactory() = default;

  PrimitiveFactory(const PrimitiveFactory&) = delete;
  PrimitiveFactory& operator=(const PrimitiveFactory&) = delete;
  PrimitiveFactory(PrimitiveFactory&&) = delete;
  PrimitiveFactory& operator=(PrimitiveFactory&&) = delete;

  /**
   * @brief Create a `Sphere` from typed parameters.
   *
   * @param [in] center   Sphere centre (default: origin).
   * @param [in] radius   Sphere radius (default: 1.0).
   * @param [in] material Surface material (default: nullptr).
   * @returns Shared pointer to the freshly constructed sphere.
   */
  [[nodiscard]] static std::shared_ptr<IObject> createSphere(
      const math::Vector3D& center = {0.0, 0.0, 0.0}, double radius = 1.0,
      std::shared_ptr<IMaterial> material = nullptr);

  /**
   * @brief Create a `Cone` from typed parameters.
   *
   * @param [in] apex     Cone apex (default: origin).
   * @param [in] axis     Cone axis (default: +Y).
   * @param [in] angle    Half-angle in radians (default: π/4).
   * @param [in] height   Cone height along the axis (default: 1.0).
   * @param [in] material Surface material (default: nullptr).
   * @returns Shared pointer to the freshly constructed cone.
   */
  [[nodiscard]] static std::shared_ptr<IObject> createCone(
      const math::Vector3D& apex = {0.0, 0.0, 0.0},
      const math::Vector3D& axis = {0.0, 1.0, 0.0},
      double angle = defaultConeAngle, double height = 1.0,
      std::shared_ptr<IMaterial> material = nullptr);

  /**
   * @brief Create a `Cylinder` from typed parameters.
   *
   * @param [in] center   Cylinder centre (default: origin).
   * @param [in] axis     Cylinder axis (default: +Y).
   * @param [in] radius   Cylinder radius (default: 1.0).
   * @param [in] height   Cylinder height along the axis (default: 1.0).
   * @param [in] material Surface material (default: nullptr).
   * @returns Shared pointer to the freshly constructed cylinder.
   */
  [[nodiscard]] static std::shared_ptr<IObject> createCylinder(
      const math::Vector3D& center = {0.0, 0.0, 0.0},
      const math::Vector3D& axis = {0.0, 1.0, 0.0}, double radius = 1.0,
      double height = 1.0, std::shared_ptr<IMaterial> material = nullptr);

  /**
   * @brief Create a `Plane` from typed parameters.
   *
   * @param [in] point    A point belonging to the plane (default: origin).
   * @param [in] normal   The plane's outward normal (default: +Y).
   * @param [in] material Surface material (default: nullptr).
   * @returns Shared pointer to the freshly constructed plane.
   */
  [[nodiscard]] static std::shared_ptr<IObject> createPlane(
      const math::Vector3D& point = {0.0, 0.0, 0.0},
      const math::Vector3D& normal = {0.0, 1.0, 0.0},
      std::shared_ptr<IMaterial> material = nullptr);

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"sphere"`, `"cone"`, `"cylinder"`, `"plane"`.
   *
   * @param [in] type Type discriminant (one of the supported names above).
   * @param [in] cfg  libconfig setting; absent fields keep their typed defaults.
   * @returns Shared pointer to the freshly constructed primitive.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<IObject> create(
      const std::string& type, const libconfig::Setting& cfg);
};

}  // namespace raytracer::core::factory

#endif  // FACTORY_PRIMITIVEFACTORY_HPP_
