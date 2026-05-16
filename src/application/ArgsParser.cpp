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

bool extractFlag(int argc, const char** argv, std::string_view flag,
                 std::string& outValue) {
  for (int i = 1; i < argc - 1; ++i) {
    if (std::string_view(argv[i]) == flag) {
      outValue = argv[i + 1];
      return true;
    }
  }
  return false;
}

bool hasFlag(int argc, const char** argv, std::string_view flag) {
  for (int i = 1; i < argc; ++i) {
    if (std::string_view(argv[i]) == flag) return true;
  }
  return false;
}

}  // namespace

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
  request.useBVH = !hasFlag(argc, argv, "--no-bvh");

  std::string configPath;
  if (extractFlag(argc, argv, "--config", configPath)) {
    request.renderConfigPath = configPath;
  }

  return request;
}

}  // namespace raytracer::core
