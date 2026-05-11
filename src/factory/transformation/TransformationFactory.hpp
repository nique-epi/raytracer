/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** TransformationFactory
*/

#pragma once

#include <memory>
#include <string>
#include "components/Transformations/ITransformation.hpp"
#include "utils/math/Vector3D.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

/**
 * @brief Static, statically-linked factory for `ITransformation` transforms.
 *
 * Exposes a typed creator per concrete transformation (translation, rotation)
 * plus a string-based `create(type, cfg)` dispatch that parses a libconfig
 * setting into typed arguments and delegates to the typed creator.
 */
class TransformationFactory {
 public:
  TransformationFactory() = default;
  ~TransformationFactory() = default;

  TransformationFactory(const TransformationFactory&) = delete;
  TransformationFactory& operator=(const TransformationFactory&) = delete;
  TransformationFactory(TransformationFactory&&) = delete;
  TransformationFactory& operator=(TransformationFactory&&) = delete;

  /**
   * @brief Create a translation transformation.
   *
   * @param [in] offset Translation offset (default: zero).
   * @returns Shared pointer to the freshly constructed transformation.
   */
  [[nodiscard]] static std::shared_ptr<ITransformation> createTranslation(
      const math::Vector3D& offset = {0.0, 0.0, 0.0});

  /**
   * @brief Create a rotation transformation.
   *
   * @param [in] axis  Rotation axis (default: +Z).
   * @param [in] angle Rotation angle in radians (default: 0.0).
   * @returns Shared pointer to the freshly constructed transformation.
   */
  [[nodiscard]] static std::shared_ptr<ITransformation> createRotation(
      const math::Vector3D& axis = {0.0, 0.0, 1.0}, double angle = 0.0);

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"translation"`, `"rotation"`.
   *
   * @param [in] type Type discriminant (one of the supported names above).
   * @param [in] cfg  libconfig setting; absent fields keep their typed defaults.
   * @returns Shared pointer to the freshly constructed transformation.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<ITransformation> create(
      const std::string& type, const libconfig::Setting& cfg);
};

}  // namespace raytracer::core::factory
