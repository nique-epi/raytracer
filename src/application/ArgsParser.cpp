/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser
*/

#include "ArgsParser.hpp"
#include <string_view>

namespace raytracer::core {

namespace {
bool applyFlag(std::string_view flag, SceneRequest& request) {
  if (flag == "--no-bvh") {
    request.useBVH = false;
    return true;
  }
#ifdef BUILD_BONUS
  if (flag == "--viewport") {
    request.viewport = true;
    return true;
  }
#endif
  return false;
}
}  // namespace

std::optional<AppConfig> ArgsParser::parse(int argc, const char** argv) {
  if (argc < 2) {
    return std::nullopt;
  }
  const std::string_view first = argv[1];
  if (first == "-h" || first == "--help") {
    if (argc != 2) {
      return std::nullopt;
    }
    return HelpRequest{};
  }

  SceneRequest request{.scenePath = argv[1]};
  for (int i = 2; i < argc; ++i) {
    if (!applyFlag(argv[i], request)) {
      return std::nullopt;
    }
  }
  return request;
}

}  // namespace raytracer::core
