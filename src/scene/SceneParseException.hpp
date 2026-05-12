/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** SceneParseException
*/

#pragma once

#include <string>
#include "scene/SceneLoaderException.hpp"

namespace raytracer::scene {

/**
 * @brief Exception thrown when a scene file contains invalid syntax.
 *
 * Wraps a `libconfig::ParseException`. Two overloads are provided:
 * one without a line number (for generic parse errors) and one with
 * a line number (for errors where libconfig reports the exact location).
 */
class SceneParseException : public SceneLoaderException {
 public:
  /**
   * @brief Construct a parse exception without a line number.
   *
   * The message follows the pattern:
   * `"scene parse error in '<path>': <reason>"`.
   *
   * @param path   Path (or file name reported by libconfig) of the scene file.
   * @param reason Human-readable description of the parse error.
   */
  SceneParseException(const std::string& path, const std::string& reason);

  /**
   * @brief Construct a parse exception with a line number.
   *
   * The message follows the pattern:
   * `"scene parse error in '<path>':<line>: <reason>"`.
   *
   * @param path   Path (or file name reported by libconfig) of the scene file.
   * @param line   Line number where the parse error was detected.
   * @param reason Human-readable description of the parse error.
   */
  SceneParseException(const std::string& path, int line,
                      const std::string& reason);
};

}  // namespace raytracer::scene
