/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ISceneLoader
*/

#pragma once
#include <string>
#include "scene/SceneBuilder.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {

/**
 * @brief Contract for any scene-file format loader (CFG, JSON, GLB, ...).
 *
 * Concrete implementations are registered in a SceneLoaderFactory and
 * selected by file extension via supports(). Loaders are stateless: they
 * mutate the builder/settings passed in and never own them.
 */
// NOLINTNEXTLINE
class ISceneLoader {
 public:
  virtual ~ISceneLoader() = default;

  /**
   * @brief Parse a scene file and populate the builder and render settings.
   *
   * On success, every primitive, light and camera declared in the file is
   * registered into @p builder, and any optional `settings` block fills
   * @p settings. On failure (I/O error, parse error), false is returned and
   * a human-readable diagnostic is written to stderr.
   *
   * @param [in]     path     Path to the scene file on disk.
   * @param [in,out] builder  Scene builder to populate.
   * @param [in,out] settings Render settings to fill (existing values are
   *                          preserved when the format omits a key).
   * @returns true  if the file was loaded successfully.
   * @returns false on I/O or parse error.
   */
  virtual bool load(const std::string& path, SceneBuilder& builder,
                    math::RenderSettings& settings) = 0;

  /**
   * @brief Test whether this loader can handle the given file extension.
   *
   * @param [in] ext File extension without the leading dot (e.g. "cfg").
   * @returns true  if the loader supports this extension.
   * @returns false otherwise.
   */
  [[nodiscard]] virtual bool supports(const std::string& ext) const = 0;
};

}  // namespace raytracer::scene
