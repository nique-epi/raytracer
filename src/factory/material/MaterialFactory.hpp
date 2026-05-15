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
#include "components/material/Textures/ITexture.hpp"
#include "utils/math/Color.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

inline constexpr double defaultGlassRefractionIndex = 1.5;
inline constexpr double defaultPrincipledRoughness = 0.5;
inline constexpr double defaultPrincipledRefractionIndex = 1.5;

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
   * @brief Create a Principled BSDF material (metallic/roughness PBR).
   *
   * @param [in] baseColor Base color albedo.
   * @param [in] metallic  Metallic factor in [0, 1] (0 = dielectric, 1 =
   * metal).
   * @param [in] roughness Roughness factor in [0, 1] (0 = mirror-like, 1 =
   * diffuse).
    * @param [in] ior       Index of refraction for dielectrics (default: 1.5).
   * @param [in] alpha     Opacity factor in [0, 1] (0 = fully transparent,
   * 1 = fully opaque).
   * @returns Shared pointer to the freshly constructed material.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> createPrincipled(
      const math::Color& baseColor, double metallic = 0.0,
      double roughness = defaultPrincipledRoughness,
      double ior = defaultPrincipledRefractionIndex, double alpha = 1.0);

  /**
   * @brief Create a textured Lambertian material.
   *
   * @param [in] texture Texture used to modulate the surface color.
   * @param [in] albedo  Base color multiplied by the texture sample (default:
   * white).
   * @returns Shared pointer to the freshly constructed material.
   */
  [[nodiscard]] static std::shared_ptr<IMaterial> createTextured(
      std::shared_ptr<raytracer::materials::ITexture> texture,
      const math::Color& albedo = math::Color(1.0, 1.0, 1.0));

  /**
   * @brief Parse an `ITexture` from a libconfig texture block.
   *
   * Supported type names: `"solid"`, `"checker"`, `"noise"`, `"bitmap"`.
   *
   * @param [in] cfg libconfig setting containing `type` and texture-specific
   * fields.
   * @returns Shared pointer to the constructed texture.
   *
   * @throws raytracer::core::RaytracerException If the type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<raytracer::materials::ITexture>
  parseTexture(const libconfig::Setting& cfg);

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"diffuse"`, `"glossy"`, `"glass"`, `"textured"`.
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
