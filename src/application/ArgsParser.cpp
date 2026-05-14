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
  if (argc == 2) {
    return SceneRequest{.scenePath = argv[1], .useBVH = true};
  }
  if (argc == 3 && std::string_view(argv[2]) == "--no-bvh") {
    return SceneRequest{.scenePath = argv[1], .useBVH = false};
  }
  return std::nullopt;
}

}  // namespace raytracer::core
