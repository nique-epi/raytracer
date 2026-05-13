/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CFGSceneLoader
*/

#pragma once
#include <libconfig.h++>
#include "ISceneLoader.hpp"

namespace raytracer::scene {

/**
 * @brief Scene loader for the libconfig++ `.cfg` text format.
 *
 * Supports the keys mandated by the project subject:
 *  - `camera`
 *  - `primitives.spheres`, `primitives.planes`
 *  - `lights.ambient`, `lights.directional`, `lights.point`
 *  - `settings` (optional) with imageWidth, imageHeight, tileWidth,
 *    tileHeight, numThreads, samplesPerPixel, maxDepth.
 *
 * Unknown primitive/light types are forwarded to SceneBuilder::addObject
 * and resolved later via the ObjectRegistry, which lets new plugins extend
 * the format without touching this loader.
 */
class CFGSceneLoader : public ISceneLoader {
 public:
  /**
   * @brief Parse a `.cfg` scene file using libconfig++.
   *
   * @param [in]     path     Path to the `.cfg` file.
   * @param [in,out] builder  Builder that receives every parsed entity.
   * @param [in,out] settings Render settings populated from the optional
   *                          `settings` block; defaults are preserved when
   *                          the block or any individual key is absent.
   * @returns true  on successful parse.
   * @returns false if the file cannot be read (FileIOException) or parsed
   *                (ParseException). In both cases a diagnostic line —
   *                including file and line number for parse errors — is
   *                written to stderr.
   */
  bool load(const std::string& path, SceneBuilder& builder,
            math::RenderSettings& settings) override;

  /**
   * @brief Indicate that this loader handles the `.cfg` extension.
   *
   * @param [in] ext File extension without the leading dot.
   * @returns true  if @p ext equals "cfg".
   * @returns false otherwise.
   */
  [[nodiscard]] bool supports(const std::string& ext) const override;

 private:
  /**
   * @brief Walk the `primitives` block and forward each entry to the builder.
   *
   * @param [in]     root    Root setting of the parsed config.
   * @param [in,out] builder Builder that receives each primitive entry.
   */
  static void parsePrimitives(const libconfig::Setting& root,
                              SceneBuilder& builder);

  /**
   * @brief Walk the `lights` block and forward each entry to the builder.
   *
   * @param [in]     root    Root setting of the parsed config.
   * @param [in,out] builder Builder that receives each light entry.
   */
  static void parseLights(const libconfig::Setting& root,
                          SceneBuilder& builder);

  /**
   * @brief Forward the `camera` block to the builder, if present.
   *
   * @param [in]     root    Root setting of the parsed config.
   * @param [in,out] builder Builder that receives the camera entry.
   */
  static void parseCamera(const libconfig::Setting& root,
                          SceneBuilder& builder);

  /**
   * @brief Apply the optional `settings` block to the render settings.
   *
   * Each key inside `settings` is independently optional: missing keys
   * leave the corresponding field of @p settings untouched.
   *
   * @param [in]     root     Root setting of the parsed config.
   * @param [in,out] settings Render settings to update in place.
   */
  static void parseSettings(const libconfig::Setting& root,
                            math::RenderSettings& settings);

  /**
   * @brief Parse the optional `background` block and attach it to the scene
   *        via the builder.
   *
   * Supported types: `"solid"`, `"gradient"`. Defaults to a black solid
   * background when the block is absent.
   *
   * @param [in]     root    Root setting of the parsed config.
   * @param [in,out] builder Builder whose scene background will be set.
   */
  static void parseBackground(const libconfig::Setting& root,
                              SceneBuilder& builder);

  /**
   * @brief Parse the optional `world` block and apply its viewport mode
   *        to the scene via the builder.
   *
   * Recognised key: `mode` with string values `"wireframe"`,
   * `"materialPreview"`, or `"rendered"` (default). Unknown values are
   * silently treated as `"rendered"` so a typo does not crash the load.
   *
   * @param [in]     root    Root setting of the parsed config.
   * @param [in,out] builder Builder whose scene World will be updated.
   */
  static void parseWorld(const libconfig::Setting& root,
                         SceneBuilder& builder);
};

}  // namespace raytracer::scene
