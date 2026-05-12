/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** MaterialFactory
*/

#pragma once

#include <memory>
#include <string>
#include "components/material/IMaterial.hpp"
#include "utils/math/Color.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

inline constexpr double defaultGlassRefractionIndex = 1.5;

/**
 * @brief Static, statically-linked factory for `IMaterial` materials.
 *
 * Exposes a typed creator per concrete material (diffuse, glossy, glass)
 * plus a string-based `create(type, cfg)` dispatch that parses a libconfig
 * setting into typed arguments and delegates to the typed creator.
 */
class MaterialFactory {
 public:
  MaterialFactory() = default;
  ~MaterialFactory() = default;

  MaterialFactory(const MaterialFactory&) = delete;
  MaterialFactory& operator=(const MaterialFactory&) = delete;
  MaterialFactory(MaterialFactory&&) = delete;
  MaterialFactory& operator=(MaterialFactory&&) = delete;

  /**
   * @brief Create a Lambertian diffuse material.
   *
   * @param [in] albedo Surface albedo (default: white).
   * @returns Shared pointer to the freshly constructed material.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> createDiffuse(
      const math::Color& albedo = math::Color(1.0, 1.0, 1.0));

  /**
   * @brief Create a glossy (slightly-rough mirror) material.
   *
   * @param [in] fuzz   Roughness in [0, 1] (default: 0.0 — perfect mirror).
   * @param [in] albedo Surface albedo (default: white).
   * @returns Shared pointer to the freshly constructed material.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> createGlossy(
      double fuzz = 0.0,
      const math::Color& albedo = math::Color(1.0, 1.0, 1.0));

  /**
   * @brief Create a dielectric (glass-like) material.
   *
   * @param [in] refractionIndex Index of refraction (default: 1.5).
   * @param [in] tint            Tint color applied when light passes through
   * (default: white).
   * @returns Shared pointer to the freshly constructed material.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> createGlass(
      double refractionIndex = defaultGlassRefractionIndex,
      const math::Color& tint = math::Color(1.0, 1.0, 1.0));

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"diffuse"`, `"glossy"`, `"glass"`.
   *
   * @param [in] type Type discriminant (one of the supported names above).
   * @param [in] cfg  libconfig setting; absent fields keep their typed
   * defaults.
   * @returns Shared pointer to the freshly constructed material.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> create(
      const std::string& type, const libconfig::Setting& cfg);

  /**
   * @brief Parse an RGB color block from @p s, keeping @p fallback for absent
   *        channels.
   *
   * Channels are read as integers in [0, 255] under keys `"r"`, `"g"`, `"b"`.
   *
   * @param [in] s        libconfig setting containing the color fields.
   * @param [in] fallback Color used when a channel key is absent.
   * @returns Normalised color with each channel in [0, 1].
   */
  [[nodiscard]] static math::Color parseColor(const libconfig::Setting& s,
                                              const math::Color& fallback);
};

}  // namespace raytracer::core::factory
