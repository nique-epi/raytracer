/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneFileNotFoundException
*/

#pragma once

#include <string>
#include "scene/SceneLoaderException.hpp"

namespace raytracer::scene {

/**
 * @brief Exception thrown when a scene file cannot be opened for reading.
 *
 * Wraps a `libconfig::FileIOException`: the path is embedded in the
 * message so callers get a human-readable diagnostic without needing to
 * inspect the original libconfig exception.
 */
class SceneFileNotFoundException : public SceneLoaderException {
 public:
  /**
   * @brief Construct the exception with the offending file path.
   *
   * The message follows the pattern:
   * `"scene file not found: '<path>'"`.
   *
   * @param path Path to the scene file that could not be opened.
   */
  explicit SceneFileNotFoundException(const std::string& path);
};

}  // namespace raytracer::scene
