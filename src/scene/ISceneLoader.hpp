/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ISceneLoader
*/

#pragma once
#include <string>
#include "utils/math/RenderSettings.hpp"

namespace raytracer::scene {

class SceneBuilder;

// NOLINTNEXTLINE
class ISceneLoader {
 public:
  virtual ~ISceneLoader() = default;

  /**
   * @brief Load a scene from a file.
   *
   * Mutates @p builder by registering all primitives, lights and materials
   * found in the file. Mutates @p settings with the render parameters
   * (resolution, samples, etc.) if the format provides them.
   *
   * @param path     Path to the scene file.
   * @param builder  Scene builder to populate.
   * @param settings Render settings to fill.
   * @return true if the file was loaded successfully, false otherwise.
   */
  virtual bool load(const std::string& path, SceneBuilder& builder,
                    math::RenderSettings& settings) = 0;
  [[nodiscard]] virtual bool supports(const std::string& ext) const = 0;
};

}  // namespace raytracer::scene
