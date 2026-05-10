/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ConfigException
*/

#pragma once

#include <string>
#include "exceptions/Exceptions.hpp"

namespace raytracer::config {

/**
 * @brief Exception thrown when a libconfig++ setting cannot be read.
 *
 * Two construction modes are provided:
 *  - missing required field: `ConfigException(field)`
 *  - invalid type or other reason: `ConfigException(field, reason)`
 *
 * Both inherit from `Core::RaytracerException` so they are caught by the
 * top-level handler in `main` and produce exit code 84.
 */
class ConfigException : public raytracer::core::RaytracerException {
 public:
  /**
   * @brief Construct a "missing required field" exception.
   *
   * The message follows the pattern:
   * `"missing required config field: '<field>'"`.
   *
   * @param field Name of the libconfig++ setting that was expected but absent.
   */
  explicit ConfigException(const std::string& field);

  /**
   * @brief Construct an "invalid type or value" exception.
   *
   * The message follows the pattern:
   * `"invalid type for config field for field '<field>': <reason>"`.
   *
   * @param field  Name of the libconfig++ setting that caused the error.
   * @param reason Human-readable description of the type or value mismatch.
   */
  ConfigException(const std::string& field, const std::string& reason);
};

}  // namespace raytracer::config
