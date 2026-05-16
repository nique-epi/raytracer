/*
** EPITECH PROJECT, 2026
** raytracer
** File description:
** ArgsParser
*/

#include "ArgsParser.hpp"
#include <string_view>

namespace raytracer::core {

std::optional<AppConfig> ArgsParser::parse(int argc, const char** argv) {
  if (argc == 2 && (std::string_view(argv[1]) == "-h" ||
                    std::string_view(argv[1]) == "--help")) {
    return HelpRequest{};
  }
  if (argc < 2) {
    return std::nullopt;
  }

  SceneRequest request;
  request.scenePath = argv[1];
  request.useBVH = true;

  int i = 2;
  while (i < argc) {
    const std::string_view arg(argv[i]);
    if (arg == "--no-bvh") {
      request.useBVH = false;
      ++i;
    } else if (arg == "--config" && i + 1 < argc) {
      request.renderConfigPath = argv[i + 1];
      i += 2;
    } else {
      return std::nullopt;
    }
  }
  return request;
}

}  // namespace raytracer::core
