/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CameraFactory
*/

#pragma once

#include <memory>
#include <string>
#include "components/camera/ICamera.hpp"
#include "components/camera/perspective/PerspectiveCamera.hpp"
#include "utils/math/Vector3D.hpp"

namespace libconfig {
class Setting;
}  // namespace libconfig

namespace raytracer::core::factory {

/**
 * @brief Static, statically-linked factory for `ICamera` cameras.
 *
 * Exposes a typed creator per concrete camera (currently perspective only)
 * plus a string-based `create(type, cfg)` dispatch that parses a libconfig
 * setting into typed arguments and delegates to the typed creator.
 */
class CameraFactory {
 public:
  CameraFactory() = default;
  ~CameraFactory() = default;

  CameraFactory(const CameraFactory&) = delete;
  CameraFactory& operator=(const CameraFactory&) = delete;
  CameraFactory(CameraFactory&&) = delete;
  CameraFactory& operator=(CameraFactory&&) = delete;

  /**
   * @brief Create a perspective camera with thin-lens parameters.
   *
   * @param [in] lookFrom    Camera position (default: origin).
   * @param [in] lookAt      Point the camera looks at (default: -Z).
   * @param [in] vup         Up direction (default: +Y).
   * @param [in] vfov        Vertical field of view in degrees (default: 90).
   * @param [in] aspectRatio Aspect ratio (default: 1.0; updated later via setResolution).
   * @param [in] aperture    Lens aperture (default: 0.0 — pinhole).
   * @param [in] focusDist   Focus distance from the camera (default: 1.0).
   * @returns Shared pointer to the freshly constructed camera.
   */
  [[nodiscard]] static std::shared_ptr<ICamera> createPerspective(
      const math::Vector3D& lookFrom = {0.0, 0.0, 0.0},
      const math::Vector3D& lookAt = {0.0, 0.0, -1.0},
      const math::Vector3D& vup = {0.0, 1.0, 0.0},
      double vfov = components::camera::perspective::DEFAULT_FOV,
      double aspectRatio = 1.0, double aperture = 0.0,
      double focusDist = 1.0);

  /**
   * @brief Dispatch by type name and parse @p cfg into typed arguments.
   *
   * Supported type names: `"perspective"`.
   *
   * Recognised libconfig keys: `position` (vec3) maps to lookFrom;
   * `direction` (vec3) is added to position to yield lookAt, OR `lookAt`
   * (vec3) overrides directly; `up` (vec3) overrides the up vector;
   * `fov` / `vfov` (double) sets the vertical field of view; `aperture`
   * and `focusDist` are doubles. Missing fields keep their defaults.
   *
   * @param [in] type Type discriminant (currently only `"perspective"`).
   * @param [in] cfg  libconfig setting describing the camera.
   * @returns Shared pointer to the freshly constructed camera.
   *
   * @throws raytracer::core::RaytracerException If @p type is unknown.
   */
  [[nodiscard]] static std::shared_ptr<ICamera> create(
      const std::string& type, const libconfig::Setting& cfg);
};

}  // namespace raytracer::core::factory
