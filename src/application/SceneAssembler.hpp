/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneAssembler
*/

/**
 * @file SceneAssembler.hpp
 * @brief Application-level scene composer.
 *
 * `SceneAssembler` owns the scene-loader factory (CFG + bonus
 * Assimp) and the component factory, and turns a scene file path
 * (plus an optional JSON render-config) into a ready-to-render
 * `LoadedScene`: parsed scene + validated render settings + resolved
 * output path. The class wraps every step of the pipeline so
 * `Application::run` keeps a single call to `assembler_.load(...)`
 * instead of a 40-line chain of loader-resolution, parsing, JSON
 * merge, validation, BVH and camera plumbing.
 */

#pragma once

#include <memory>
#include <optional>
#include <string>

#include "factory/ComponentFactory.hpp"
#include "scene/SceneLoaderFactory.hpp"
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {
class Scene;
}  // namespace raytracer::scene

namespace raytracer::core {

/**
 * @brief Aggregate of everything `SceneAssembler::load` produces:
 *        a parsed scene, validated render settings, and the output
 *        path the renderer should write to.
 */
struct LoadedScene {
  std::shared_ptr<scene::Scene> scene;
  math::RenderSettings settings;
  std::string outputPath;
};

class SceneAssembler {
 public:
  /**
   * @brief Build an assembler with the project's default loaders
   *        already registered.
   *
   * Registers the CFG loader unconditionally. Registers the Assimp
   * loader when compiled with `BUILD_BONUS`. No other I/O happens at
   * construction.
   */
  SceneAssembler();
  ~SceneAssembler() = default;

  SceneAssembler(const SceneAssembler&) = delete;
  SceneAssembler& operator=(const SceneAssembler&) = delete;
  SceneAssembler(SceneAssembler&&) = delete;
  SceneAssembler& operator=(SceneAssembler&&) = delete;

  /**
   * @brief Load, validate and post-process a scene from @p scenePath.
   *
   * Resolves the right scene loader for the file extension, parses
   * the scene file, merges JSON overrides (`BUILD_BONUS` only), runs
   * `RenderSettings::validate()`, builds a BVH when @p useBVH is
   * `true` and pushes the resolution to the camera.
   *
   * @param[in] scenePath        Path to the scene file (`.cfg`,
   *                             `.gltf`, …).
   * @param[in] useBVH           Build a BVH after the scene is parsed.
   * @param[in] renderConfigPath Optional path to a JSON render-config
   *                             file. Ignored unless compiled with
   *                             `BUILD_BONUS`.
   * @returns A ready-to-render `LoadedScene`.
   * @throws raytracer::core::RaytracerException When no loader matches
   *         the extension, the file cannot be parsed, the settings
   *         are invalid, or a JSON config was provided without
   *         `BUILD_BONUS`.
   */
  LoadedScene load(const std::string& scenePath, bool useBVH,
                   const std::optional<std::string>& renderConfigPath);

 private:
  scene::SceneLoaderFactory sceneLoaderFactory_;
  factory::ComponentFactory componentFactory_;
};

}  // namespace raytracer::core
