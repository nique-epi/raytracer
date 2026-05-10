/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** LightFactory
*/

#ifndef FACTORY_LIGHTFACTORY_HPP_
#define FACTORY_LIGHTFACTORY_HPP_

#include <memory>
#include <string>
#include "components/light/ILight.hpp"
#include "utils/math/Color.hpp"
#include "utils/math/Vector3D.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

/**
 * @brief Static, statically-linked factory for `ILight` lights.
 *
 * Exposes a typed creator per concrete light (ambient, directional) plus a
 * string-based `create(type, cfg)` dispatch that parses a libconfig setting
 * into the matching typed arguments and delegates to the typed creator.
 *
 * Each typed creator carries its own default arguments; the string
 * dispatcher seeds local variables with the same defaults and only
 * overrides those actually present in @p cfg.
 */
class LightFactory {
 public:
  LightFactory() = default;
  ~LightFactory() = default;

  LightFactory(const LightFactory&) = delete;
  LightFactory& operator=(const LightFactory&) = delete;
  LightFactory(LightFactory&&) = delete;
  LightFactory& operator=(LightFactory&&) = delete;

  /**
   * @brief Create an ambient light from typed parameters.
   *
   * @param [in] color     Light colour (default: white {1,1,1}).
   * @param [in] intensity Scalar multiplier (default: 1.0).
   * @returns Shared pointer to the freshly constructed ambient light.
   */
  [[nodiscard]] static std::shared_ptr<ILight> createAmbient(
      const math::Color& color = math::Color(1.0, 1.0, 1.0),
      double intensity = 1.0);

  /**
   * @brief Create a directional light from typed parameters.
   *
   * @param [in] direction Light propagation direction (default: -Y).
   * @param [in] color     Light colour (default: white {1,1,1}).
   * @param [in] intensity Scalar multiplier (default: 1.0).
   * @returns Shared pointer to the freshly constructed directional light.
   */
  [[nodiscard]] static std::shared_ptr<ILight> createDirectional(
      const math::Vector3D& direction = {0.0, -1.0, 0.0},
      const math::Color& color = math::Color(1.0, 1.0, 1.0),
      double intensity = 1.0);

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"ambient"`, `"directional"`.
   *
   * @param [in] type Type discriminant (one of the supported names above).
   * @param [in] cfg  libconfig setting; absent fields keep their typed defaults.
   * @returns Shared pointer to the freshly constructed light.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<ILight> create(
      const std::string& type, const libconfig::Setting& cfg);
};

}  // namespace raytracer::core::factory

#endif  // FACTORY_LIGHTFACTORY_HPP_
