/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** JsonSettingsLoader
*/

#pragma once

#include <optional>
#include <string>
#include "scene/World.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::bonus::json {

/**
 * @brief Holds the result of parsing a JSON settings file.
 *
 * `settings` carries every numeric render parameter (resolution, tile size,
 * thread count, sample count, max depth). `viewportMode` is set only when the
 * JSON file explicitly contains the `"viewportMode"` key; a missing key leaves
 * it as `std::nullopt` so the value parsed from the scene file is preserved.
 */
struct JsonSettings {
  math::RenderSettings settings;
  std::optional<raytracer::scene::ViewportMode> viewportMode;
  std::optional<std::string> outputFile;
};

/**
 * @brief Loads a JSON settings file into a `JsonSettings`.
 *
 * The JSON file must be a flat object whose keys are a subset of:
 * `"imageWidth"`, `"imageHeight"`, `"tileWidth"`, `"tileHeight"`,
 * `"numThreads"`, `"samplesPerPixel"`, `"maxDepth"`, `"viewportMode"`,
 * `"name"`.
 * All keys are optional; absent keys keep the `RenderSettings` defaults.
 *
 * `"viewportMode"` accepts the strings `"rendered"`, `"wireframe"`, and
 * `"materialPreview"`. Unknown values are treated as `"rendered"`.
 */
class JsonSettingsLoader {
 public:
  /**
   * @brief Parse @p path into a `JsonSettings`, starting from @p base.
   *
   * Keys present in the JSON file override the corresponding field of @p base.
   * Keys absent from the JSON file leave the @p base value untouched, so
   * settings already parsed from a scene file are preserved.
   *
   * @param [in] path Path to the JSON settings file.
   * @param [in] base Starting settings (typically populated from the scene
   *                  file). Defaults to a default-constructed
   *                  `RenderSettings`.
   * @returns Populated `JsonSettings`.
   * @throws raytracer::scene::SceneFileNotFoundException If the file cannot be
   *         opened.
   * @throws raytracer::scene::SceneParseException If the JSON is malformed or
   *         contains type errors.
   */
  [[nodiscard]] static JsonSettings load(
      const std::string& path,
      const math::RenderSettings& base = math::RenderSettings{});

  /**
   * @brief Push the values held by @p json into @p settings, @p scene and
   *        @p outputPath.
   *
   * Always overwrites @p settings with `json.settings` (the JSON is the
   * authoritative source once a config file is provided). Conditionally
   * overrides the scene's `viewportMode` and the caller's @p outputPath
   * only when the JSON had those keys set (i.e. when the matching
   * optional is engaged); a missing key never clobbers the value
   * coming from the scene file or the caller's default.
   *
   * @param[in,out] scene      Scene whose `World::viewportMode` may be
   *                           overridden.
   * @param[in,out] settings   Render settings replaced wholesale by
   *                           `json.settings`.
   * @param[in]     json       JSON-derived overrides.
   * @param[in,out] outputPath Output path that may be replaced.
   */
  static void applyOverrides(raytracer::scene::Scene& scene,
                             math::RenderSettings& settings,
                             const JsonSettings& json,
                             std::string& outputPath);
};

}  // namespace raytracer::bonus::json
