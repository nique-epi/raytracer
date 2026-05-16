/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser
*/

#pragma once
#include <optional>
#include <string>
#include <variant>

namespace raytracer::core {

struct HelpRequest {};

/**
 * @brief Parsed command-line request to run a scene.
 *
 * Built by `ArgsParser::parse` from `argv`. Owns no resources.
 * The `viewport` field is only present when the project is built with
 * `BUILD_VIEWPORT=ON`. It defaults to true (the SFML live-display
 * window opens unless `--no-viewport` is passed).
 */
struct SceneRequest {
  std::string scenePath;
  bool useBVH{true};
#ifdef BUILD_VIEWPORT
  bool viewport{true};
#endif
};

using AppConfig = std::variant<HelpRequest, SceneRequest>;

class ArgsParser {
 public:
  /**
   * @brief Parse the CLI invocation.
   *
   * Recognised forms:
   *   raytracer -h | --help
   *   raytracer <SCENE_FILE> [--no-bvh] [--no-viewport]
   *
   * `--no-viewport` is only accepted when compiled with
   * `BUILD_VIEWPORT`. Flags after the scene path may appear in any
   * order. Any unknown flag yields `std::nullopt` (usage error).
   *
   * @param [in] argc Argument count from `main`.
   * @param [in] argv Argument vector from `main`.
   * @returns `HelpRequest` if help was requested, a `SceneRequest` on a
   *          valid invocation, or `std::nullopt` on a parse error.
   */
  [[nodiscard]] static std::optional<AppConfig> parse(int argc,
                                                      const char** argv);
};

}  // namespace raytracer::core
