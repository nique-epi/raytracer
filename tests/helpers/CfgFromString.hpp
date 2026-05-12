/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** CfgFromString helper for libconfig-driven unit tests
*/

#pragma once

#include <libconfig.h++>

namespace raytracer::tests {

/**
 * @brief Build a libconfig setting tree from an inline source string.
 *
 * Each test that exercises a libconfig-driven factory needs a small
 * `libconfig::Setting` tree to feed it. This helper parses an inline
 * string at construction time so test bodies stay focused on the case
 * under test, without touching the filesystem.
 */
class CfgFromString {
 public:
  /**
   * @brief Parse @p source as a libconfig document.
   *
   * @param [in] source The full libconfig source text.
   */
  explicit CfgFromString(const char* source) { config_.readString(source); }

  /**
   * @brief Access the parsed document's root group.
   */
  [[nodiscard]] const libconfig::Setting& root() const {
    return config_.getRoot();
  }

  /**
   * @brief Look up a setting by absolute path inside the parsed document.
   *
   * @param [in] path A libconfig path expression (e.g. `"camera"`).
   * @returns The setting at @p path.
   *
   * @throws libconfig::SettingNotFoundException If @p path is missing.
   */
  [[nodiscard]] const libconfig::Setting& at(const char* path) const {
    return config_.lookup(path);
  }

 private:
  libconfig::Config config_;
};

}  // namespace raytracer::tests
